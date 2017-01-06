#include "minimal/stdlib.h"

#include <ffi.h>

#include "sig/ffi_type.h"
#include "sig/types.h"

ffi_type *sig_objc_ffi_type(apr_pool_t *pool, struct sig_type *type) {
    switch (type->primitive) {
        case typename_P: return &ffi_type_pointer;

        case union_P:
            /* XXX: we can totally make this work */
            _assert(false);
        break;

        case string_P: return &ffi_type_pointer;
        case selector_P: return &ffi_type_pointer;
        case object_P: return &ffi_type_pointer;
        case boolean_P: return &ffi_type_uchar;
        case uchar_P: return &ffi_type_uchar;
        case uint_P: return &ffi_type_uint;
        case ulong_P: return &ffi_type_ulong;
        case ulonglong_P: return &ffi_type_ulong;
        case ushort_P: return &ffi_type_ushort;

        case array_P:
            /* XXX: implement */
            _assert(false);
        break;

        case pointer_P: return &ffi_type_pointer;

        case bit_P:
            /* XXX: we can totally make this work */
            _assert(false);
        break;

        case char_P: return &ffi_type_schar;
        case double_P: return &ffi_type_double;
        case float_P: return &ffi_type_float;
        case int_P: return &ffi_type_sint;
        case long_P: return &ffi_type_sint;
        case longlong_P: return &ffi_type_slong;
        case short_P: return &ffi_type_sshort;

        case void_P: return &ffi_type_void;

        case struct_P: {
            ffi_type *aggregate = apr_palloc(pool, sizeof(ffi_type));
            aggregate->size = 0;
            aggregate->alignment = 0;
            aggregate->type = FFI_TYPE_STRUCT;

            aggregate->elements = apr_palloc(pool, (type->data.signature.count + 1) * sizeof(ffi_type *));
            sig_ffi_types(pool, &sig_objc_ffi_type, &type->data.signature, aggregate->elements, 0, 0);
            aggregate->elements[type->data.signature.count] = NULL;

            return aggregate;
        } break;

        default:
            _assert(false);
        break;
    }
}

ffi_type *sig_java_ffi_type(apr_pool_t *pool, struct sig_type *type) {
    switch (type->primitive) {
        case typename_P: return &ffi_type_pointer;
        case union_P: return &ffi_type_pointer;
        case string_P: return &ffi_type_pointer;
        case selector_P: return &ffi_type_pointer;
        case object_P: return &ffi_type_pointer;
        case boolean_P: return &ffi_type_uchar;
        case uchar_P: return &ffi_type_uchar;
        case uint_P: return &ffi_type_uint;
        case ulong_P: return &ffi_type_ulong;
        case ulonglong_P: return &ffi_type_ulong;
        case ushort_P: return &ffi_type_ushort;
        case array_P: return &ffi_type_pointer;
        case pointer_P: return &ffi_type_pointer;

        /* XXX: bit type */
        case bit_P: return &ffi_type_uint;

        case char_P: return &ffi_type_schar;
        case double_P: return &ffi_type_double;
        case float_P: return &ffi_type_double;
        case int_P: return &ffi_type_sint;
        case long_P: return &ffi_type_sint;
        case longlong_P: return &ffi_type_slong;
        case short_P: return &ffi_type_sshort;
        case void_P: return &ffi_type_void;
        case struct_P: return &ffi_type_pointer;

        default:
            _assert(false);
        break;
    }
}

void sig_ffi_types(
    apr_pool_t *pool,
    ffi_type *(*sig_ffi_type)(apr_pool_t *, struct sig_type *),
    struct sig_signature *signature,
    ffi_type **ffi_types,
    size_t skip,
    size_t offset
) {
    _assert(signature->count >= skip);
    for (size_t index = skip; index != signature->count; ++index)
        ffi_types[index - skip + offset] = (*sig_ffi_type)(pool, signature->elements[index].type);
}

void sig_ffi_cif(
    apr_pool_t *pool,
    ffi_type *(*sig_ffi_type)(apr_pool_t *, struct sig_type *),
    struct sig_signature *signature,
    ffi_cif *ffi_cif,
    ffi_type **types,
    size_t skip,
    size_t offset
) {
    ffi_type *type = (*sig_ffi_type)(pool, signature->elements[0].type);
    sig_ffi_types(pool, sig_ffi_type, signature, types, 1 + skip, offset);
    ffi_status status = ffi_prep_cif(ffi_cif, FFI_DEFAULT_ABI, signature->count - 1 - skip + offset, type, types);
    _assert(status == FFI_OK);
}
