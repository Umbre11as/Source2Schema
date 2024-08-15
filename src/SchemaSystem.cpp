#include "SchemaSystem.h"

CUtlVector<const SchemaScope*> SchemaSystem::Scopes() const {
    return scopes;
}

const SchemaScope* SchemaSystem::Get(const char* scopeName) const {
    for (SIZE_T i = 0; i < scopes.Size; i++)
        if (strcmp(scopes[i]->Name(), scopeName) == 0)
            return scopes[i];

    return nullptr;
}

PCSTR SchemaScope::Name() const {
    return name;
}

const CUtlVector<const SchemaClass*> SchemaScope::Classes() const {
    static CUtlVector<const SchemaClass*> classes{};
    classes.Size = declaredClassesCount;
    classes.Buffer = new const SchemaClass*[declaredClassesCount];

    for (WORD i = 0; i < declaredClassesCount; i++) {
        const SchemaDeclaredClassEntry entry = declaredClasses[i];
        const SchemaDeclaredClass* declaredClass = entry.DeclaredClass();
        SchemaClass* pClass = declaredClass->Class();

        classes.Buffer[i] = pClass;
    }

    return classes;
}

const SchemaClass* SchemaScope::Get(const char* className) const {
    const CUtlVector<const SchemaClass*> classes = Classes();
    for (SIZE_T i = 0; i < classes.Size; i++)
        if (strcmp(classes.Buffer[i]->Name(), className) == 0)
            return classes.Buffer[i];

    return nullptr;
}

bool SchemaClass::FindField(const char* name, SchemaField* field) const {
    const std::vector<SchemaField> fields = Fields();
    for (SIZE_T i = 0; i < fields.size(); i++) {
        if (strcmp(fields[i].Name(), name) == 0) {
            *field = fields[i];
            return true;
        }
    }

    return false;
}

PCSTR SchemaClass::Name() const {
    return name;
}

PCSTR SchemaClass::ScopeName() const {
    return scopeName;
}

UINT SchemaClass::Size() const {
    return size;
}

std::vector<SchemaField> SchemaClass::Fields() const {
    std::vector<SchemaField> fields(fieldCount);

    for (WORD i = 0; i < fieldCount; i++)
        fields[i] = declaredFields[i];

    return fields;
}

PCSTR SchemaField::Name() const {
    return name;
}

PVOID SchemaField::Type() const {
    return type;
}

UINT SchemaField::Offset() const {
    return offset;
}

UINT SchemaField::MetadataSize() const {
    return metadataSize;
}

PVOID SchemaField::Metadata() const {
    return metadata;
}
