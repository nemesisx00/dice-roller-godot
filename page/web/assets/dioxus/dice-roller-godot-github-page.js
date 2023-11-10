import { create, update_memory, save_template, get_node, initilize } from './snippets/dioxus-interpreter-js-603636eeca72cf05/inline0.js';
import { setAttributeInner } from './snippets/dioxus-interpreter-js-603636eeca72cf05/src/common.js';
import { get_form_data } from './snippets/dioxus-web-54817add8ba334eb/inline0.js';
import { Dioxus } from './snippets/dioxus-web-54817add8ba334eb/src/eval.js';
import * as __wbg_star0 from './snippets/dioxus-interpreter-js-603636eeca72cf05/inline0.js';

let wasm;

const heap = new Array(128).fill(undefined);

heap.push(undefined, null, true, false);

function getObject(idx) { return heap[idx]; }

let heap_next = heap.length;

function dropObject(idx) {
    if (idx < 132) return;
    heap[idx] = heap_next;
    heap_next = idx;
}

function takeObject(idx) {
    const ret = getObject(idx);
    dropObject(idx);
    return ret;
}

let WASM_VECTOR_LEN = 0;

let cachedUint8Memory0 = null;

function getUint8Memory0() {
    if (cachedUint8Memory0 === null || cachedUint8Memory0.byteLength === 0) {
        cachedUint8Memory0 = new Uint8Array(wasm.memory.buffer);
    }
    return cachedUint8Memory0;
}

const cachedTextEncoder = (typeof TextEncoder !== 'undefined' ? new TextEncoder('utf-8') : { encode: () => { throw Error('TextEncoder not available') } } );

const encodeString = (typeof cachedTextEncoder.encodeInto === 'function'
    ? function (arg, view) {
    return cachedTextEncoder.encodeInto(arg, view);
}
    : function (arg, view) {
    const buf = cachedTextEncoder.encode(arg);
    view.set(buf);
    return {
        read: arg.length,
        written: buf.length
    };
});

function passStringToWasm0(arg, malloc, realloc) {

    if (typeof(arg) !== 'string') throw new Error('expected a string argument');

    if (realloc === undefined) {
        const buf = cachedTextEncoder.encode(arg);
        const ptr = malloc(buf.length, 1) >>> 0;
        getUint8Memory0().subarray(ptr, ptr + buf.length).set(buf);
        WASM_VECTOR_LEN = buf.length;
        return ptr;
    }

    let len = arg.length;
    let ptr = malloc(len, 1) >>> 0;

    const mem = getUint8Memory0();

    let offset = 0;

    for (; offset < len; offset++) {
        const code = arg.charCodeAt(offset);
        if (code > 0x7F) break;
        mem[ptr + offset] = code;
    }

    if (offset !== len) {
        if (offset !== 0) {
            arg = arg.slice(offset);
        }
        ptr = realloc(ptr, len, len = offset + arg.length * 3, 1) >>> 0;
        const view = getUint8Memory0().subarray(ptr + offset, ptr + len);
        const ret = encodeString(arg, view);
        if (ret.read !== arg.length) throw new Error('failed to pass whole string');
        offset += ret.written;
    }

    WASM_VECTOR_LEN = offset;
    return ptr;
}

function isLikeNone(x) {
    return x === undefined || x === null;
}

let cachedInt32Memory0 = null;

function getInt32Memory0() {
    if (cachedInt32Memory0 === null || cachedInt32Memory0.byteLength === 0) {
        cachedInt32Memory0 = new Int32Array(wasm.memory.buffer);
    }
    return cachedInt32Memory0;
}

const cachedTextDecoder = (typeof TextDecoder !== 'undefined' ? new TextDecoder('utf-8', { ignoreBOM: true, fatal: true }) : { decode: () => { throw Error('TextDecoder not available') } } );

if (typeof TextDecoder !== 'undefined') { cachedTextDecoder.decode(); };

function getStringFromWasm0(ptr, len) {
    ptr = ptr >>> 0;
    return cachedTextDecoder.decode(getUint8Memory0().subarray(ptr, ptr + len));
}

function addHeapObject(obj) {
    if (heap_next === heap.length) heap.push(heap.length + 1);
    const idx = heap_next;
    heap_next = heap[idx];

    if (typeof(heap_next) !== 'number') throw new Error('corrupt heap');

    heap[idx] = obj;
    return idx;
}

function _assertBoolean(n) {
    if (typeof(n) !== 'boolean') {
        throw new Error('expected a boolean argument');
    }
}

function _assertNum(n) {
    if (typeof(n) !== 'number') throw new Error('expected a number argument');
}

let cachedFloat64Memory0 = null;

function getFloat64Memory0() {
    if (cachedFloat64Memory0 === null || cachedFloat64Memory0.byteLength === 0) {
        cachedFloat64Memory0 = new Float64Array(wasm.memory.buffer);
    }
    return cachedFloat64Memory0;
}

function _assertBigInt(n) {
    if (typeof(n) !== 'bigint') throw new Error('expected a bigint argument');
}

let cachedBigInt64Memory0 = null;

function getBigInt64Memory0() {
    if (cachedBigInt64Memory0 === null || cachedBigInt64Memory0.byteLength === 0) {
        cachedBigInt64Memory0 = new BigInt64Array(wasm.memory.buffer);
    }
    return cachedBigInt64Memory0;
}

function debugString(val) {
    // primitive types
    const type = typeof val;
    if (type == 'number' || type == 'boolean' || val == null) {
        return  `${val}`;
    }
    if (type == 'string') {
        return `"${val}"`;
    }
    if (type == 'symbol') {
        const description = val.description;
        if (description == null) {
            return 'Symbol';
        } else {
            return `Symbol(${description})`;
        }
    }
    if (type == 'function') {
        const name = val.name;
        if (typeof name == 'string' && name.length > 0) {
            return `Function(${name})`;
        } else {
            return 'Function';
        }
    }
    // objects
    if (Array.isArray(val)) {
        const length = val.length;
        let debug = '[';
        if (length > 0) {
            debug += debugString(val[0]);
        }
        for(let i = 1; i < length; i++) {
            debug += ', ' + debugString(val[i]);
        }
        debug += ']';
        return debug;
    }
    // Test for built-in
    const builtInMatches = /\[object ([^\]]+)\]/.exec(toString.call(val));
    let className;
    if (builtInMatches.length > 1) {
        className = builtInMatches[1];
    } else {
        // Failed to match the standard '[object ClassName]'
        return toString.call(val);
    }
    if (className == 'Object') {
        // we're a user defined class or Object
        // JSON.stringify avoids problems with cycles, and is generally much
        // easier than looping through ownProperties of `val`.
        try {
            return 'Object(' + JSON.stringify(val) + ')';
        } catch (_) {
            return 'Object';
        }
    }
    // errors
    if (val instanceof Error) {
        return `${val.name}: ${val.message}\n${val.stack}`;
    }
    // TODO we could test for more things here, like `Set`s and `Map`s.
    return className;
}

function makeMutClosure(arg0, arg1, dtor, f) {
    const state = { a: arg0, b: arg1, cnt: 1, dtor };
    const real = (...args) => {
        // First up with a closure we increment the internal reference
        // count. This ensures that the Rust closure environment won't
        // be deallocated while we're invoking it.
        state.cnt++;
        const a = state.a;
        state.a = 0;
        try {
            return f(a, state.b, ...args);
        } finally {
            if (--state.cnt === 0) {
                wasm.__wbindgen_export_2.get(state.dtor)(a, state.b);

            } else {
                state.a = a;
            }
        }
    };
    real.original = state;

    return real;
}

function logError(f, args) {
    try {
        return f.apply(this, args);
    } catch (e) {
        let error = (function () {
            try {
                return e instanceof Error ? `${e.message}\n\nStack:\n${e.stack}` : e.toString();
            } catch(_) {
                return "<failed to stringify thrown value>";
            }
        }());
        console.error("wasm-bindgen: imported JS function that was not marked as `catch` threw an error:", error);
        throw e;
    }
}
function __wbg_adapter_48(arg0, arg1) {
    _assertNum(arg0);
    _assertNum(arg1);
    wasm._dyn_core__ops__function__FnMut_____Output___R_as_wasm_bindgen__closure__WasmClosure___describe__invoke__h3fe26f6858c8a6ed(arg0, arg1);
}

let stack_pointer = 128;

function addBorrowedObject(obj) {
    if (stack_pointer == 1) throw new Error('out of js stack');
    heap[--stack_pointer] = obj;
    return stack_pointer;
}
function __wbg_adapter_51(arg0, arg1, arg2) {
    try {
        _assertNum(arg0);
        _assertNum(arg1);
        wasm._dyn_core__ops__function__FnMut___A____Output___R_as_wasm_bindgen__closure__WasmClosure___describe__invoke__h33d0597416295a80(arg0, arg1, addBorrowedObject(arg2));
    } finally {
        heap[stack_pointer++] = undefined;
    }
}

function __wbg_adapter_54(arg0, arg1, arg2) {
    _assertNum(arg0);
    _assertNum(arg1);
    wasm._dyn_core__ops__function__FnMut__A____Output___R_as_wasm_bindgen__closure__WasmClosure___describe__invoke__he98cc7b5d9f76cca(arg0, arg1, addHeapObject(arg2));
}

function __wbg_adapter_57(arg0, arg1, arg2) {
    _assertNum(arg0);
    _assertNum(arg1);
    wasm._dyn_core__ops__function__FnMut__A____Output___R_as_wasm_bindgen__closure__WasmClosure___describe__invoke__hb69d4d8ecbac26b2(arg0, arg1, addHeapObject(arg2));
}

function getCachedStringFromWasm0(ptr, len) {
    if (ptr === 0) {
        return getObject(len);
    } else {
        return getStringFromWasm0(ptr, len);
    }
}

let cachedUint32Memory0 = null;

function getUint32Memory0() {
    if (cachedUint32Memory0 === null || cachedUint32Memory0.byteLength === 0) {
        cachedUint32Memory0 = new Uint32Array(wasm.memory.buffer);
    }
    return cachedUint32Memory0;
}

function getArrayJsValueFromWasm0(ptr, len) {
    ptr = ptr >>> 0;
    const mem = getUint32Memory0();
    const slice = mem.subarray(ptr / 4, ptr / 4 + len);
    const result = [];
    for (let i = 0; i < slice.length; i++) {
        result.push(takeObject(slice[i]));
    }
    return result;
}

function handleError(f, args) {
    try {
        return f.apply(this, args);
    } catch (e) {
        wasm.__wbindgen_exn_store(addHeapObject(e));
    }
}

async function __wbg_load(module, imports) {
    if (typeof Response === 'function' && module instanceof Response) {
        if (typeof WebAssembly.instantiateStreaming === 'function') {
            try {
                return await WebAssembly.instantiateStreaming(module, imports);

            } catch (e) {
                if (module.headers.get('Content-Type') != 'application/wasm') {
                    console.warn("`WebAssembly.instantiateStreaming` failed because your server does not serve wasm with `application/wasm` MIME type. Falling back to `WebAssembly.instantiate` which is slower. Original error:\n", e);

                } else {
                    throw e;
                }
            }
        }

        const bytes = await module.arrayBuffer();
        return await WebAssembly.instantiate(bytes, imports);

    } else {
        const instance = await WebAssembly.instantiate(module, imports);

        if (instance instanceof WebAssembly.Instance) {
            return { instance, module };

        } else {
            return instance;
        }
    }
}

function __wbg_get_imports() {
    const imports = {};
    imports.wbg = {};
    imports.wbg.__wbindgen_object_drop_ref = function(arg0) {
        takeObject(arg0);
    };
    imports.wbg.__wbindgen_string_get = function(arg0, arg1) {
        const obj = getObject(arg1);
        const ret = typeof(obj) === 'string' ? obj : undefined;
        var ptr1 = isLikeNone(ret) ? 0 : passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
        var len1 = WASM_VECTOR_LEN;
        getInt32Memory0()[arg0 / 4 + 1] = len1;
        getInt32Memory0()[arg0 / 4 + 0] = ptr1;
    };
    imports.wbg.__wbindgen_error_new = function(arg0, arg1) {
        const ret = new Error(getStringFromWasm0(arg0, arg1));
        return addHeapObject(ret);
    };
    imports.wbg.__wbg_new_86c764e2105c4284 = function() { return logError(function (arg0) {
        const ret = new Dioxus(getObject(arg0));
        return addHeapObject(ret);
    }, arguments) };
    imports.wbg.__wbindgen_is_undefined = function(arg0) {
        const ret = getObject(arg0) === undefined;
        _assertBoolean(ret);
        return ret;
    };
    imports.wbg.__wbg_rustSend_aa102e1dd65dd988 = function() { return logError(function (arg0, arg1) {
        getObject(arg0).rustSend(takeObject(arg1));
    }, arguments) };
    imports.wbg.__wbindgen_boolean_get = function(arg0) {
        const v = getObject(arg0);
        const ret = typeof(v) === 'boolean' ? (v ? 1 : 0) : 2;
        _assertNum(ret);
        return ret;
    };
    imports.wbg.__wbindgen_is_bigint = function(arg0) {
        const ret = typeof(getObject(arg0)) === 'bigint';
        _assertBoolean(ret);
        return ret;
    };
    imports.wbg.__wbindgen_bigint_from_i64 = function(arg0) {
        const ret = arg0;
        return addHeapObject(ret);
    };
    imports.wbg.__wbindgen_jsval_eq = function(arg0, arg1) {
        const ret = getObject(arg0) === getObject(arg1);
        _assertBoolean(ret);
        return ret;
    };
    imports.wbg.__wbindgen_number_get = function(arg0, arg1) {
        const obj = getObject(arg1);
        const ret = typeof(obj) === 'number' ? obj : undefined;
        if (!isLikeNone(ret)) {
            _assertNum(ret);
        }
        getFloat64Memory0()[arg0 / 8 + 1] = isLikeNone(ret) ? 0 : ret;
        getInt32Memory0()[arg0 / 4 + 0] = !isLikeNone(ret);
    };
    imports.wbg.__wbindgen_is_object = function(arg0) {
        const val = getObject(arg0);
        const ret = typeof(val) === 'object' && val !== null;
        _assertBoolean(ret);
        return ret;
    };
    imports.wbg.__wbindgen_in = function(arg0, arg1) {
        const ret = getObject(arg0) in getObject(arg1);
        _assertBoolean(ret);
        return ret;
    };
    imports.wbg.__wbindgen_bigint_from_u64 = function(arg0) {
        const ret = BigInt.asUintN(64, arg0);
        return addHeapObject(ret);
    };
    imports.wbg.__wbindgen_is_string = function(arg0) {
        const ret = typeof(getObject(arg0)) === 'string';
        _assertBoolean(ret);
        return ret;
    };
    imports.wbg.__wbindgen_object_clone_ref = function(arg0) {
        const ret = getObject(arg0);
        return addHeapObject(ret);
    };
    imports.wbg.__wbindgen_string_new = function(arg0, arg1) {
        const ret = getStringFromWasm0(arg0, arg1);
        return addHeapObject(ret);
    };
    imports.wbg.__wbg_getformdata_30aff64ff4a9a080 = function() { return logError(function (arg0) {
        const ret = get_form_data(getObject(arg0));
        return addHeapObject(ret);
    }, arguments) };
    imports.wbg.__wbg_new_abda76e883ba8a5f = function() { return logError(function () {
        const ret = new Error();
        return addHeapObject(ret);
    }, arguments) };
    imports.wbg.__wbg_stack_658279fe44541cf6 = function() { return logError(function (arg0, arg1) {
        const ret = getObject(arg1).stack;
        const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
        const len1 = WASM_VECTOR_LEN;
        getInt32Memory0()[arg0 / 4 + 1] = len1;
        getInt32Memory0()[arg0 / 4 + 0] = ptr1;
    }, arguments) };
    imports.wbg.__wbg_error_f851667af71bcfc6 = function() { return logError(function (arg0, arg1) {
        var v0 = getCachedStringFromWasm0(arg0, arg1);
    if (arg0 !== 0) { wasm.__wbindgen_free(arg0, arg1, 1); }
    console.error(v0);
}, arguments) };
imports.wbg.__wbindgen_cb_drop = function(arg0) {
    const obj = takeObject(arg0).original;
    if (obj.cnt-- == 1) {
        obj.a = 0;
        return true;
    }
    const ret = false;
    _assertBoolean(ret);
    return ret;
};
imports.wbg.__wbindgen_number_new = function(arg0) {
    const ret = arg0;
    return addHeapObject(ret);
};
imports.wbg.__wbindgen_jsval_loose_eq = function(arg0, arg1) {
    const ret = getObject(arg0) == getObject(arg1);
    _assertBoolean(ret);
    return ret;
};
imports.wbg.__wbg_String_88810dfeb4021902 = function() { return logError(function (arg0, arg1) {
    const ret = String(getObject(arg1));
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_set_841ac57cff3d672b = function() { return logError(function (arg0, arg1, arg2) {
    getObject(arg0)[takeObject(arg1)] = takeObject(arg2);
}, arguments) };
imports.wbg.__wbg_create_4fa2dd8b14b6c3cc = function() { return logError(function (arg0) {
    create(arg0 >>> 0);
}, arguments) };
imports.wbg.__wbg_updatememory_4c09c2246a86718f = function() { return logError(function (arg0) {
    update_memory(takeObject(arg0));
}, arguments) };
imports.wbg.__wbg_savetemplate_742f86aebd69bb83 = function() { return logError(function (arg0, arg1, arg2) {
    var v0 = getArrayJsValueFromWasm0(arg0, arg1).slice();
    wasm.__wbindgen_free(arg0, arg1 * 4, 4);
    save_template(v0, arg2 >>> 0);
}, arguments) };
imports.wbg.__wbg_getnode_b1633a4b84264ce7 = function() { return logError(function (arg0) {
    const ret = get_node(arg0 >>> 0);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_initilize_8b525fbe950133cd = function() { return logError(function (arg0, arg1) {
    initilize(takeObject(arg0), getObject(arg1));
}, arguments) };
imports.wbg.__wbg_setAttributeInner_ce265aa8339405b1 = function() { return logError(function (arg0, arg1, arg2, arg3, arg4, arg5) {
    var v0 = getCachedStringFromWasm0(arg1, arg2);
    var v1 = getCachedStringFromWasm0(arg4, arg5);
    setAttributeInner(takeObject(arg0), v0, takeObject(arg3), v1);
}, arguments) };
imports.wbg.__wbg_instanceof_Window_e266f02eee43b570 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof Window;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_document_950215a728589a2d = function() { return logError(function (arg0) {
    const ret = getObject(arg0).document;
    return isLikeNone(ret) ? 0 : addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_createElement_e2a0e21263eb5416 = function() { return handleError(function (arg0, arg1, arg2) {
    var v0 = getCachedStringFromWasm0(arg1, arg2);
    const ret = getObject(arg0).createElement(v0);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_createElementNS_0047de728927ea00 = function() { return handleError(function (arg0, arg1, arg2, arg3, arg4) {
    var v0 = getCachedStringFromWasm0(arg1, arg2);
    var v1 = getCachedStringFromWasm0(arg3, arg4);
    const ret = getObject(arg0).createElementNS(v0, v1);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_createTextNode_866e33a51b47f04c = function() { return logError(function (arg0, arg1, arg2) {
    var v0 = getCachedStringFromWasm0(arg1, arg2);
    const ret = getObject(arg0).createTextNode(v0);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_getElementById_eb93a47327bb5585 = function() { return logError(function (arg0, arg1, arg2) {
    var v0 = getCachedStringFromWasm0(arg1, arg2);
    const ret = getObject(arg0).getElementById(v0);
    return isLikeNone(ret) ? 0 : addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_instanceof_CompositionEvent_57e2f1c0e7330ddc = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof CompositionEvent;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_data_70b505c651722930 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).data;
    var ptr1 = isLikeNone(ret) ? 0 : passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    var len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_top_98bc5382de3ec5d2 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).top;
    return ret;
}, arguments) };
imports.wbg.__wbg_left_09342c290d46b4a6 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).left;
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_HtmlFormElement_04e7484e36bd99d6 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof HTMLFormElement;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_HtmlTextAreaElement_4bc39f9d861a6832 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof HTMLTextAreaElement;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_value_00fb0fdc46959169 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).value;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_width_f0cbf7dcbbe056da = function() { return logError(function (arg0) {
    const ret = getObject(arg0).width;
    return ret;
}, arguments) };
imports.wbg.__wbg_height_e46975153da440ae = function() { return logError(function (arg0) {
    const ret = getObject(arg0).height;
    return ret;
}, arguments) };
imports.wbg.__wbg_length_e330009c21d43b27 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).length;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_item_d2f164053a173c14 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg0).item(arg1 >>> 0);
    return isLikeNone(ret) ? 0 : addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_screenX_efa7b61dc7fa2efd = function() { return logError(function (arg0) {
    const ret = getObject(arg0).screenX;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_screenY_c5e6449919709a69 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).screenY;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_clientX_35f23f953e04ec0e = function() { return logError(function (arg0) {
    const ret = getObject(arg0).clientX;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_clientY_8104e462abc0b3ec = function() { return logError(function (arg0) {
    const ret = getObject(arg0).clientY;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_offsetX_413d9f02022e72ad = function() { return logError(function (arg0) {
    const ret = getObject(arg0).offsetX;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_offsetY_488f80a0a9666028 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).offsetY;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_ctrlKey_e1b8f1de1eb24d5d = function() { return logError(function (arg0) {
    const ret = getObject(arg0).ctrlKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_shiftKey_fdd99b6df96e25c5 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).shiftKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_altKey_d531a4d3704557cb = function() { return logError(function (arg0) {
    const ret = getObject(arg0).altKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_metaKey_934772989e28020c = function() { return logError(function (arg0) {
    const ret = getObject(arg0).metaKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_button_a1c470d5e4c997f2 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).button;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_buttons_42a7b7de33d8e572 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).buttons;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_animationName_8b08b6b55e2871c3 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).animationName;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_elapsedTime_b9b2848f0ce69c4d = function() { return logError(function (arg0) {
    const ret = getObject(arg0).elapsedTime;
    return ret;
}, arguments) };
imports.wbg.__wbg_pseudoElement_489395cbb841b9a8 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).pseudoElement;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_result_4c6690478b5532e4 = function() { return handleError(function (arg0) {
    const ret = getObject(arg0).result;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_setonload_14ae8c68b7ee43ee = function() { return logError(function (arg0, arg1) {
    getObject(arg0).onload = getObject(arg1);
}, arguments) };
imports.wbg.__wbg_new_8eef8a8754c6aae7 = function() { return handleError(function () {
    const ret = new FileReader();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_readAsArrayBuffer_bc9f4aff6d3e1bb1 = function() { return handleError(function (arg0, arg1) {
    getObject(arg0).readAsArrayBuffer(getObject(arg1));
}, arguments) };
imports.wbg.__wbg_readAsText_c143a5f37567d018 = function() { return handleError(function (arg0, arg1) {
    getObject(arg0).readAsText(getObject(arg1));
}, arguments) };
imports.wbg.__wbg_pointerId_d2caae4465ba386f = function() { return logError(function (arg0) {
    const ret = getObject(arg0).pointerId;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_width_a1d5efe9db3fb17a = function() { return logError(function (arg0) {
    const ret = getObject(arg0).width;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_height_7a964b61a7a42a7d = function() { return logError(function (arg0) {
    const ret = getObject(arg0).height;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_pressure_352c13794490720b = function() { return logError(function (arg0) {
    const ret = getObject(arg0).pressure;
    return ret;
}, arguments) };
imports.wbg.__wbg_tangentialPressure_1dfc978ca995fe64 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).tangentialPressure;
    return ret;
}, arguments) };
imports.wbg.__wbg_tiltX_5e065070f9907bab = function() { return logError(function (arg0) {
    const ret = getObject(arg0).tiltX;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_tiltY_88a95794fa24061b = function() { return logError(function (arg0) {
    const ret = getObject(arg0).tiltY;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_twist_1ac0d3bf085b9fa5 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).twist;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_pointerType_df759fa0bd6634ed = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).pointerType;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_isPrimary_31079f1bab7f6665 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).isPrimary;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_altKey_088afaf4c4a6fe7c = function() { return logError(function (arg0) {
    const ret = getObject(arg0).altKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_metaKey_69f3ca388ab2c6b2 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).metaKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_ctrlKey_011662ff21669501 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).ctrlKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_shiftKey_5472f6ec6b0da088 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).shiftKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_propertyName_e41c6239ffb77f2a = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).propertyName;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_elapsedTime_6bbd8686761d767f = function() { return logError(function (arg0) {
    const ret = getObject(arg0).elapsedTime;
    return ret;
}, arguments) };
imports.wbg.__wbg_pseudoElement_589db5e026c64890 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).pseudoElement;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_deltaX_b7d127c94d6265c0 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).deltaX;
    return ret;
}, arguments) };
imports.wbg.__wbg_deltaY_b32fa858e16edcc0 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).deltaY;
    return ret;
}, arguments) };
imports.wbg.__wbg_deltaZ_f74840dd94a2e4c0 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).deltaZ;
    return ret;
}, arguments) };
imports.wbg.__wbg_deltaMode_11f7b19e64d9a515 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).deltaMode;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_Element_cb847a3fc7b1b1a4 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof Element;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_getAttribute_2c20e00a5cd314af = function() { return logError(function (arg0, arg1, arg2, arg3) {
    var v0 = getCachedStringFromWasm0(arg2, arg3);
    const ret = getObject(arg1).getAttribute(v0);
    var ptr2 = isLikeNone(ret) ? 0 : passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    var len2 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len2;
    getInt32Memory0()[arg0 / 4 + 0] = ptr2;
}, arguments) };
imports.wbg.__wbg_getBoundingClientRect_aaa701cbcb448965 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).getBoundingClientRect();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_scrollIntoView_4176ef6d7f93fca3 = function() { return logError(function (arg0, arg1) {
    getObject(arg0).scrollIntoView(getObject(arg1));
}, arguments) };
imports.wbg.__wbg_toggleAttribute_0d026e5820a28711 = function() { return handleError(function (arg0, arg1, arg2) {
    var v0 = getCachedStringFromWasm0(arg1, arg2);
    const ret = getObject(arg0).toggleAttribute(v0);
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_HtmlElement_9e442d53bb553421 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof HTMLElement;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_blur_b94a30ed202c3612 = function() { return handleError(function (arg0) {
    getObject(arg0).blur();
}, arguments) };
imports.wbg.__wbg_focus_6497e1b44dabfb24 = function() { return handleError(function (arg0) {
    getObject(arg0).focus();
}, arguments) };
imports.wbg.__wbg_instanceof_HtmlInputElement_5c9d54338207f061 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof HTMLInputElement;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_checked_44c09d0c819e33ad = function() { return logError(function (arg0) {
    const ret = getObject(arg0).checked;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_files_e7db01553b30ef33 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).files;
    return isLikeNone(ret) ? 0 : addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_type_6bd11acdabd07813 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).type;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_value_1f2c9e357d18d3ea = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).value;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_name_ccf3024ae4e3ac54 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).name;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_altKey_dff2a075455ac01b = function() { return logError(function (arg0) {
    const ret = getObject(arg0).altKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_ctrlKey_993b558f853d64ce = function() { return logError(function (arg0) {
    const ret = getObject(arg0).ctrlKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_shiftKey_31e62e9d172b26f0 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).shiftKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_metaKey_9f0f19692d0498bd = function() { return logError(function (arg0) {
    const ret = getObject(arg0).metaKey;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_location_e0f6bf3e5e23180a = function() { return logError(function (arg0) {
    const ret = getObject(arg0).location;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_repeat_7ef48474c78e74bd = function() { return logError(function (arg0) {
    const ret = getObject(arg0).repeat;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_key_f0decac219aa904b = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).key;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_code_aed21120de275a12 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).code;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_type_1dff9d19be2750ce = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).type;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_target_b629c177f9bee3da = function() { return logError(function (arg0) {
    const ret = getObject(arg0).target;
    return isLikeNone(ret) ? 0 : addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_preventDefault_16b2170b12f56317 = function() { return logError(function (arg0) {
    getObject(arg0).preventDefault();
}, arguments) };
imports.wbg.__wbg_instanceof_Node_abf5312af68f179e = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof Node;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_parentElement_0e8c9afce5cb9d6e = function() { return logError(function (arg0) {
    const ret = getObject(arg0).parentElement;
    return isLikeNone(ret) ? 0 : addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_textContent_dff59ad5e030bb86 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).textContent;
    var ptr1 = isLikeNone(ret) ? 0 : passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    var len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_appendChild_b8199dc1655c852d = function() { return handleError(function (arg0, arg1) {
    const ret = getObject(arg0).appendChild(getObject(arg1));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_pageX_fa6e927410d8f9a8 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).pageX;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_pageY_fcdd161e44399498 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).pageY;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_HtmlSelectElement_d22585b1943c6b08 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof HTMLSelectElement;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_value_1b15c45090422f7f = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg1).value;
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
}, arguments) };
imports.wbg.__wbg_charCodeAt_d78a90aced59e02f = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg0).charCodeAt(arg1 >>> 0);
    return ret;
}, arguments) };
imports.wbg.__wbg_get_27fe3dac1c4d0224 = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg0)[arg1 >>> 0];
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_length_e498fbc24f9c1d4f = function() { return logError(function (arg0) {
    const ret = getObject(arg0).length;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_new_b525de17f44a8943 = function() { return logError(function () {
    const ret = new Array();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbindgen_is_function = function(arg0) {
    const ret = typeof(getObject(arg0)) === 'function';
    _assertBoolean(ret);
    return ret;
};
imports.wbg.__wbg_newnoargs_2b8b6bd7753c76ba = function() { return logError(function (arg0, arg1) {
    var v0 = getCachedStringFromWasm0(arg0, arg1);
    const ret = new Function(v0);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_new_f841cc6f2098f4b5 = function() { return logError(function () {
    const ret = new Map();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_next_b7d530c04fd8b217 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).next;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_next_88560ec06a094dea = function() { return handleError(function (arg0) {
    const ret = getObject(arg0).next();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_done_1ebec03bbd919843 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).done;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_value_6ac8da5cc5b3efda = function() { return logError(function (arg0) {
    const ret = getObject(arg0).value;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_iterator_55f114446221aa5a = function() { return logError(function () {
    const ret = Symbol.iterator;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_get_baf4855f9a986186 = function() { return handleError(function (arg0, arg1) {
    const ret = Reflect.get(getObject(arg0), getObject(arg1));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_call_95d1ea488d03e4e8 = function() { return handleError(function (arg0, arg1) {
    const ret = getObject(arg0).call(getObject(arg1));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_new_f9876326328f45ed = function() { return logError(function () {
    const ret = new Object();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_length_ea0846e494e3b16e = function() { return logError(function (arg0) {
    const ret = getObject(arg0).length;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_self_e7c1f827057f6584 = function() { return handleError(function () {
    const ret = self.self;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_window_a09ec664e14b1b81 = function() { return handleError(function () {
    const ret = window.window;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_globalThis_87cbb8506fecf3a9 = function() { return handleError(function () {
    const ret = globalThis.globalThis;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_global_c85a9259e621f3db = function() { return handleError(function () {
    const ret = global.global;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_set_17224bc548dd1d7b = function() { return logError(function (arg0, arg1, arg2) {
    getObject(arg0)[arg1 >>> 0] = takeObject(arg2);
}, arguments) };
imports.wbg.__wbg_isArray_39d28997bf6b96b4 = function() { return logError(function (arg0) {
    const ret = Array.isArray(getObject(arg0));
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_ArrayBuffer_a69f02ee4c4f5065 = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof ArrayBuffer;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_newwithargs_d66a68ef9c159f0d = function() { return logError(function (arg0, arg1, arg2, arg3) {
    var v0 = getCachedStringFromWasm0(arg0, arg1);
    var v1 = getCachedStringFromWasm0(arg2, arg3);
    const ret = new Function(v0, v1);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_call_9495de66fdbe016b = function() { return handleError(function (arg0, arg1, arg2) {
    const ret = getObject(arg0).call(getObject(arg1), getObject(arg2));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_set_388c4c6422704173 = function() { return logError(function (arg0, arg1, arg2) {
    const ret = getObject(arg0).set(getObject(arg1), getObject(arg2));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_entries_003a3a46f730c8a8 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).entries();
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_isSafeInteger_8c4789029e885159 = function() { return logError(function (arg0) {
    const ret = Number.isSafeInteger(getObject(arg0));
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_entries_4e1315b774245952 = function() { return logError(function (arg0) {
    const ret = Object.entries(getObject(arg0));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_resolve_fd40f858d9db1a04 = function() { return logError(function (arg0) {
    const ret = Promise.resolve(getObject(arg0));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_then_ec5db6d509eb475f = function() { return logError(function (arg0, arg1) {
    const ret = getObject(arg0).then(getObject(arg1));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_buffer_cf65c07de34b9a08 = function() { return logError(function (arg0) {
    const ret = getObject(arg0).buffer;
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_new_537b7341ce90bb31 = function() { return logError(function (arg0) {
    const ret = new Uint8Array(getObject(arg0));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbg_set_17499e8aa4003ebd = function() { return logError(function (arg0, arg1, arg2) {
    getObject(arg0).set(getObject(arg1), arg2 >>> 0);
}, arguments) };
imports.wbg.__wbg_length_27a2afe8ab42b09f = function() { return logError(function (arg0) {
    const ret = getObject(arg0).length;
    _assertNum(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_instanceof_Uint8Array_01cebe79ca606cca = function() { return logError(function (arg0) {
    let result;
    try {
        result = getObject(arg0) instanceof Uint8Array;
    } catch (_) {
        result = false;
    }
    const ret = result;
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_set_6aa458a4ebdb65cb = function() { return handleError(function (arg0, arg1, arg2) {
    const ret = Reflect.set(getObject(arg0), getObject(arg1), getObject(arg2));
    _assertBoolean(ret);
    return ret;
}, arguments) };
imports.wbg.__wbg_stringify_029a979dfb73aa17 = function() { return handleError(function (arg0) {
    const ret = JSON.stringify(getObject(arg0));
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbindgen_bigint_get_as_i64 = function(arg0, arg1) {
    const v = getObject(arg1);
    const ret = typeof(v) === 'bigint' ? v : undefined;
    if (!isLikeNone(ret)) {
        _assertBigInt(ret);
    }
    getBigInt64Memory0()[arg0 / 8 + 1] = isLikeNone(ret) ? BigInt(0) : ret;
    getInt32Memory0()[arg0 / 4 + 0] = !isLikeNone(ret);
};
imports.wbg.__wbindgen_debug_string = function(arg0, arg1) {
    const ret = debugString(getObject(arg1));
    const ptr1 = passStringToWasm0(ret, wasm.__wbindgen_malloc, wasm.__wbindgen_realloc);
    const len1 = WASM_VECTOR_LEN;
    getInt32Memory0()[arg0 / 4 + 1] = len1;
    getInt32Memory0()[arg0 / 4 + 0] = ptr1;
};
imports.wbg.__wbindgen_throw = function(arg0, arg1) {
    throw new Error(getStringFromWasm0(arg0, arg1));
};
imports.wbg.__wbindgen_memory = function() {
    const ret = wasm.memory;
    return addHeapObject(ret);
};
imports.wbg.__wbindgen_closure_wrapper287 = function() { return logError(function (arg0, arg1, arg2) {
    const ret = makeMutClosure(arg0, arg1, 65, __wbg_adapter_48);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbindgen_closure_wrapper288 = function() { return logError(function (arg0, arg1, arg2) {
    const ret = makeMutClosure(arg0, arg1, 65, __wbg_adapter_51);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbindgen_closure_wrapper291 = function() { return logError(function (arg0, arg1, arg2) {
    const ret = makeMutClosure(arg0, arg1, 65, __wbg_adapter_54);
    return addHeapObject(ret);
}, arguments) };
imports.wbg.__wbindgen_closure_wrapper466 = function() { return logError(function (arg0, arg1, arg2) {
    const ret = makeMutClosure(arg0, arg1, 175, __wbg_adapter_57);
    return addHeapObject(ret);
}, arguments) };
imports['./snippets/dioxus-interpreter-js-603636eeca72cf05/inline0.js'] = __wbg_star0;

return imports;
}

function __wbg_init_memory(imports, maybe_memory) {

}

function __wbg_finalize_init(instance, module) {
    wasm = instance.exports;
    __wbg_init.__wbindgen_wasm_module = module;
    cachedBigInt64Memory0 = null;
    cachedFloat64Memory0 = null;
    cachedInt32Memory0 = null;
    cachedUint32Memory0 = null;
    cachedUint8Memory0 = null;

    wasm.__wbindgen_start();
    return wasm;
}

function initSync(module) {
    if (wasm !== undefined) return wasm;

    const imports = __wbg_get_imports();

    __wbg_init_memory(imports);

    if (!(module instanceof WebAssembly.Module)) {
        module = new WebAssembly.Module(module);
    }

    const instance = new WebAssembly.Instance(module, imports);

    return __wbg_finalize_init(instance, module);
}

async function __wbg_init(input) {
    if (wasm !== undefined) return wasm;


    const imports = __wbg_get_imports();

    if (typeof input === 'string' || (typeof Request === 'function' && input instanceof Request) || (typeof URL === 'function' && input instanceof URL)) {
        input = fetch(input);
    }

    __wbg_init_memory(imports);

    const { instance, module } = await __wbg_load(await input, imports);

    return __wbg_finalize_init(instance, module);
}

export { initSync }
export default __wbg_init;
