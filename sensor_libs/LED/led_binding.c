#include "mico.h"
#include "led_binding.h"

#define led_binding_log(format, ...) custom_log("LED_binding", format, ##__VA_ARGS__)

/* Led.prototype.blink */
duk_ret_t led_blink(duk_context *ctx)
{
    /* Stack at entry is: [ ] */

    /* Get access to the 'this' binding. */
    duk_push_this(ctx); /* -> stack: [ this ] */

    /* Read this.pin */
    duk_get_prop_string(ctx, -1, "pin");
    mico_gpio_t pin = duk_to_number(ctx, -1);

    MicoGpioOutputLow(pin);
    mico_thread_msleep(500);
    MicoGpioOutputHigh(pin);
    mico_thread_msleep(500);

    return 0; /* no return value (= undefined) */
}

/* Led constructor */
duk_ret_t led_constructor(duk_context *ctx)
{
    /* Stack at entry is: [ pin ] */

    /* All Duktape/C functions can be called both as constructors
     * ("new func()") and functions ("func()").  Sometimes objects
     * allow both calls, sometimes not.  Here we reject a normal
     * non-constructor call.
     */
    if (!duk_is_constructor_call(ctx))
    {
        return DUK_RET_TYPE_ERROR;
    }

    /* Get access to the default instance. */
    duk_push_this(ctx); /* -> stack: [ pin this ] */

    /* Set this.pin to pin. */
    duk_dup(ctx, 0);                          /* -> stack: [ pin this pin ] */
    mico_gpio_t pin = duk_to_number(ctx, -1); /* -> stack: [ pin this pin ] */
    duk_put_prop_string(ctx, -2, "pin");      /* -> stack: [ pin this ] */

    /* Return undefined: default instance will be used. */
    return 0;
}

/* Initialize Led into global object. */
void led_binding_init(duk_context *ctx)
{
    /* Push constructor function; all Duktape/C functions are
     * "constructable" and can be called as 'new Foo()'.
     */
    duk_push_c_function(ctx, led_constructor, 1 /*nargs*/);

    /* Push Led.prototype object. */
    duk_push_object(ctx); /* -> stack: [ Led proto ] */

    /* Set Led.prototype.readsync. */
    duk_push_c_function(ctx, led_blink, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "blink");

    /* Set Led.prototype = proto */
    duk_put_prop_string(ctx, -2, "prototype"); /* -> stack: [ Led ] */

    /* Finally, register Led to the global object */
    duk_put_global_string(ctx, "Led"); /* -> stack: [ ] */
}