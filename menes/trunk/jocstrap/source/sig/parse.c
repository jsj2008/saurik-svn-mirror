#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "minimal/stdlib.h"

#include <apr_strings.h>

#include <string.h>

#include "sig/parse.h"

void (*sig_aggregate)(apr_pool_t *pool, enum joc_primitive primitive, const char *name, struct sig_signature *signature, const char *types) = NULL;

/* XXX: I really screwed up this time */
void *prealloc_(apr_pool_t *pool, void *odata, size_t osize, size_t nsize) {
    void *ndata = apr_palloc(pool, nsize);
    memcpy(ndata, odata, osize);
    return ndata;
}

void sig_parse_signature(apr_pool_t *pool, struct sig_signature *signature, const char **name, char eos) {
    _assert(*name != NULL);

    bool named = **name == '"';

    signature->elements = NULL;
    signature->count = 0;

    for (;;) {
        if (**name == eos) {
            ++*name;
            return;
        }

        signature->elements = (struct sig_element *) prealloc_(pool, signature->elements, signature->count * sizeof(struct sig_element), (signature->count + 1) * sizeof(struct sig_element));
        _assert(signature->elements != NULL);

        struct sig_element *element = &signature->elements[signature->count++];

        if (**name != '"')
            element->name = NULL;
        else {
            char *quote = strchr(++*name, '"');
            element->name = apr_pstrmemdup(pool, *name, quote - *name);
            *name = quote + 1;
        }

        element->type = sig_parse_type(pool, name, eos, named);

        if (**name < '0' || **name > '9')
            element->offset = _not(size_t);
        else {
            element->offset = 0;

            do
                element->offset = element->offset * 10 + (*(*name)++ - '0');
            while (**name >= '0' && **name <= '9');
        }
    }
}

struct sig_type *sig_parse_type(apr_pool_t *pool, const char **name, char eos, bool named) {
    char next = *(*name)++;
    if (next == '?')
        return NULL;

    struct sig_type *type = (struct sig_type *) apr_palloc(pool, sizeof(struct sig_type));
    _assert(type != NULL);
    memset(type, 0, sizeof(struct sig_type));

  parse:
    switch (next) {
        case '#': type->primitive = typename_P; break;

        case '(':
            type->primitive = union_P;
            next = ')';
        goto aggregate;

        case '*': type->primitive = string_P; break;
        case ':': type->primitive = selector_P; break;

        case '@':
            if (**name == '"') {
                char *quote = strchr(*name + 1, '"');
                if (!named || quote[1] == eos || quote[1] == '"') {
                    type->name = apr_pstrmemdup(pool, *name + 1, quote - *name - 1);
                    *name = quote + 1;
                }
            }

            type->primitive = object_P;
        break;

        case 'B': type->primitive = boolean_P; break;
        case 'C': type->primitive = uchar_P; break;
        case 'I': type->primitive = uint_P; break;
        case 'L': type->primitive = ulong_P; break;
        case 'Q': type->primitive = ulonglong_P; break;
        case 'S': type->primitive = ushort_P; break;

        case '[':
            type->primitive = array_P;
            type->data.data.size = strtoul(*name, (char **) name, 10);
            type->data.data.type = sig_parse_type(pool, name, eos, false);
            if (**name != ']') {
                printf("']' != \"%s\"\n", *name);
                _assert(false);
            }
            ++*name;
        break;

        case '^':
            type->primitive = pointer_P;
            if (**name == 'v') {
                type->data.data.type = NULL;
                ++*name;
            } else if (**name == '"') {
                type->data.data.type = NULL;
            } else {
                type->data.data.type = sig_parse_type(pool, name, eos, named);
            }
        break;

        case 'b':
            type->primitive = bit_P;
            type->data.data.size = strtoul(*name, (char **) name, 10);
        break;

        case 'c': type->primitive = char_P; break;
        case 'd': type->primitive = double_P; break;
        case 'f': type->primitive = float_P; break;
        case 'i': type->primitive = int_P; break;
        case 'l': type->primitive = long_P; break;
        case 'q': type->primitive = longlong_P; break;
        case 's': type->primitive = short_P; break;
        case 'v': type->primitive = void_P; break;

        case '{':
            type->primitive = struct_P;
            next = '}';
        goto aggregate;

        aggregate: {
            char end = next;
            const char *begin = *name;
            do next = *(*name)++;
            while (
                next != '=' &&
                next != '}'
            );
            size_t length = *name - begin - 1;
            if (strncmp(begin, "?", length) != 0)
                type->name = (char *) apr_pstrmemdup(pool, begin, length);
            else
                type->name = NULL;

            char *types;
            if (next != '=')
                types = NULL;
            else {
                const char *temp = *name;
                sig_parse_signature(pool, &type->data.signature, name, end);
                types = (char *) apr_pstrmemdup(pool, temp, *name - temp - 1);
            }

            if (type->name != NULL && sig_aggregate != NULL) {
                char *angle = strchr(type->name, '<');
                if (angle == NULL)
                    (*sig_aggregate)(pool, type->primitive, type->name, &type->data.signature, types);
                else {
                    angle = (char *) apr_pstrmemdup(pool, type->name, angle - type->name);
                    (*sig_aggregate)(pool, type->primitive, angle, &type->data.signature, types);
                }
            }
        } break;

        case 'N': type->flags |= JOC_TYPE_INOUT; goto next;
        case 'n': type->flags |= JOC_TYPE_IN; goto next;
        case 'O': type->flags |= JOC_TYPE_BYCOPY; goto next;
        case 'o': type->flags |= JOC_TYPE_OUT; goto next;
        case 'R': type->flags |= JOC_TYPE_BYREF; goto next;
        case 'r': type->flags |= JOC_TYPE_CONST; goto next;
        case 'V': type->flags |= JOC_TYPE_ONEWAY; goto next;

        next:
            next = *(*name)++;
            goto parse;
        break;

        default:
            printf("invalid type character: '%c' {%s}\n", next, *name - 10);
            _assert(false);
    }

    return type;
}

void sig_parse(apr_pool_t *pool, struct sig_signature *signature, const char *name) {
    const char *temp = name;
    sig_parse_signature(pool, signature, &temp, '\0');
    _assert(temp[-1] == '\0');
}

const char *sig_unparse_signature(apr_pool_t *pool, struct sig_signature *signature) {
    const char *value = "";
    size_t offset;

    for (offset = 0; offset != signature->count; ++offset) {
        const char *type = sig_unparse_type(pool, signature->elements[offset].type);
        value = apr_pstrcat(pool, value, type, NULL);
    }

    return value;
}

const char *sig_unparse_type(apr_pool_t *pool, struct sig_type *type) {
    if (type == NULL)
        return "?";
    else switch (type->primitive) {
        case typename_P: return "#";
        case union_P: return apr_psprintf(pool, "(%s)", sig_unparse_signature(pool, &type->data.signature));
        case string_P: return "*";
        case selector_P: return ":";
        case object_P: return type->name == NULL ? "@" : apr_psprintf(pool, "@\"%s\"", type->name);
        case boolean_P: return "B";
        case uchar_P: return "C";
        case uint_P: return "I";
        case ulong_P: return "L";
        case ulonglong_P: return "Q";
        case ushort_P: return "S";

        case array_P: {
            const char *value = sig_unparse_type(pool, type->data.data.type);
            return apr_psprintf(pool, "[%lu%s]", type->data.data.size, value);
        } break;

        case pointer_P: return apr_psprintf(pool, "^%s", type->data.data.type == NULL ? "" : sig_unparse_type(pool, type->data.data.type));
        case bit_P: return apr_psprintf(pool, "b%zu", type->data.data.size);
        case char_P: return "c";
        case double_P: return "d";
        case float_P: return "f";
        case int_P: return "i";
        case long_P: return "l";
        case longlong_P: return "q";
        case short_P: return "s";
        case void_P: return "v";
        case struct_P: return apr_psprintf(pool, "{%s=%s}", type->name == NULL ? "?" : type->name, sig_unparse_signature(pool, &type->data.signature));
    }

    _assert(false);
    return NULL;
}
