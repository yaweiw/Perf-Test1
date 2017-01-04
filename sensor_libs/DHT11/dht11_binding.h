#ifndef __DHT11_BINDING_H_
#define __DHT11_BINDING_H_

#include "duktapert/duktape.h"

#ifdef __cplusplus
extern "C" {
#endif
    duk_ret_t dht11_constructor(duk_context *ctx);
    duk_ret_t dht11_readsync(duk_context *ctx);

    void dht11_binding_init(duk_context *ctx);
#ifdef __cplusplus
}
#endif

#endif /* __DHT11_BINDING_H_ */