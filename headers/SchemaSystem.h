#pragma once

#include "Interface.h"
#include <type_traits>
#include <exception>
#include <vector>

#define PADDING_INSERT(x, y) x ## y
#define PADDING_DEFINE(x, y) PADDING_INSERT(x, y)
#define PRIVATE_PADDING(size) private: \
    BYTE PADDING_DEFINE(__pad_, __LINE__)[size];
#define PADDING(size) PRIVATE_PADDING(size) \
    public: \

template<typename T>
struct CUtlVector {
public:
    T& operator[](unsigned int index) {
        return Buffer[index];
    }

    T& operator[](unsigned int index) const {
        return Buffer[index];
    }
public:
    SIZE_T Size;
    T* Buffer;
};

class SchemaField {
public:
    PCSTR Name() const;

    PVOID Type() const;

    UINT Offset() const;

    UINT MetadataSize() const;

    PVOID Metadata() const;
private:
    PCSTR name;
    PVOID type;
    UINT offset;
    UINT metadataSize;
    PVOID metadata;
};

class SchemaClass {
public:
    PCSTR Name() const;

    PCSTR ScopeName() const;

    UINT Size() const;

    std::vector<SchemaField> Fields() const;

    template<typename T>
    T Get(IN PCSTR name) const {
        SchemaField field{};
        if (!FindField(name, &field))
            return {};

        if constexpr (std::is_base_of_v<SchemaField, T>)
            return field;
        else if (std::is_same_v<UINT, T>)
            return field.Offset();
        else
            throw std::exception("Type is not supported! Use one of: SchemaField or unsigned int\n");
    }
private:
    bool FindField(IN const char* name, OUT SchemaField* field) const;
private:
    PVOID vfptr;
    PCSTR name;
    PCSTR scopeName;
    UINT size;
    WORD fieldCount;
    WORD unknown0;
    WORD unknown1;
    WORD unknown2;
    UINT unknown3;
    SchemaField* declaredFields;
};

class SchemaScope {
public:
    PCSTR Name() const;

    const CUtlVector<const SchemaClass*> Classes() const;

    const SchemaClass* Get(IN const char* className) const;
private:
    class SchemaDeclaredClass {
    public:
        constexpr SchemaClass* Class() const {
            return pClass;
        }
    private:
        PVOID vfptr;
        PCSTR name;
        PCSTR unknown0;
        PCSTR unknown1;
        SchemaClass* pClass;
    };

    class SchemaDeclaredClassEntry {
    public:
        constexpr SchemaDeclaredClass* DeclaredClass() const {
            return declaredClass;
        }
    private:
        ULONGLONG hash[2];
        SchemaDeclaredClass* declaredClass;
    };
private:
    PVOID vfptr;
    CHAR name[256];
    PRIVATE_PADDING(0x338);
    SchemaDeclaredClassEntry* declaredClasses;
    PRIVATE_PADDING(0xE);
    WORD declaredClassesCount;
};

enum class Scopes {
    Client,
    Engine2,
    SchemaSystem,
    Tier0,
#if defined(SCHEMA_CS2)
    Matchmaking,
#endif
};

class SchemaSystem {
public:
    CUtlVector<const SchemaScope*> Scopes() const;

    const SchemaScope* Get(IN const char* scopeName) const;

    template<enum Scopes scope>
    const SchemaScope* Get() const {
        if constexpr (scope == Scopes::Client)
            return Get("client.dll");
        else if constexpr (scope == Scopes::Engine2)
            return Get("engine2.dll");
        else if constexpr (scope == Scopes::Tier0)
            return Get("tier0.dll");
#if defined(SCHEMA_CS2)
        else if constexpr (scope == Scopes::Matchmaking)
            return Get("matchmaking.dll");
#endif
        else
            return nullptr;
    }
private:
PRIVATE_PADDING(0x188); // @ida: sigscan 41 0F B7 BD + 4
    CUtlVector<const SchemaScope*> scopes{};
public:
    static const SchemaSystem* Create() {
        return CreateInterface<SchemaSystem>("schemasystem.dll", "SchemaSystem_001");
    }
};
