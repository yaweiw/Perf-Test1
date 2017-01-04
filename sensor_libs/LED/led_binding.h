#ifndef __LED_BINDING_H_
#define __LED_BINDING_H_

#include "duktapert/duktape.h"

#ifdef __cplusplus
extern "C" {
#endif
    duk_ret_t led_constructor(duk_context *ctx);
    duk_ret_t led_blink(duk_context *ctx);

    void led_binding_init(duk_context *ctx);
#ifdef __cplusplus
}
#endif

#endif /* __DHT11_BINDING_H_ */