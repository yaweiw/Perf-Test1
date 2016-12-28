/**
 ******************************************************************************
 * @file    hello_world.c
 * @author  William Xu
 * @version V1.0.0
 * @date    21-May-2015
 * @brief   First MiCO application to say hello world!
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */

#include "mico.h"
#include "duktape.h"
#include "app_httpd.h"

#define os_helloworld_log(format, ...)  custom_log("helloworld", format, ##__VA_ARGS__)

int config_mico_wifi( mico_Context_t* context, char* ssid, char* pass )
{
  int ssid_len = strlen(ssid);
  os_helloworld_log("SSID %s, length %d.", ssid, ssid_len);
  if (ssid_len == 0 || ssid_len > maxSsidLen - 1)
  {
    return 1;
  }
  strncpy(context->flashContentInRam.micoSystemConfig.ssid, ssid, ssid_len + 1);

  int pass_len = strlen(pass);
  strncpy(context->flashContentInRam.micoSystemConfig.key, pass, pass_len);
  strncpy(context->flashContentInRam.micoSystemConfig.user_key, pass, pass_len);
  context->flashContentInRam.micoSystemConfig.keyLength = strlen(context->flashContentInRam.micoSystemConfig.key);
  context->flashContentInRam.micoSystemConfig.user_keyLength = strlen(context->flashContentInRam.micoSystemConfig.key);

  context->flashContentInRam.micoSystemConfig.channel = 0;
  memset(context->flashContentInRam.micoSystemConfig.bssid, 0x0, 6);
  context->flashContentInRam.micoSystemConfig.security = SECURITY_TYPE_AUTO;
  context->flashContentInRam.micoSystemConfig.dhcpEnable = true;

  context->flashContentInRam.micoSystemConfig.configured = allConfigured;
  mico_system_context_update(context);
  return 0;
}

int application_start( void )
{
  /* Start MiCO system functions according to mico_config.h*/
  mico_Context_t* mico_context = (mico_Context_t*) mico_system_context_init( 0 );

  // Work around for configure WiFi now. Web-based config is blocked due to limited resource on MK3165.
  // Please modify SSID and password to your target WIFI.
  config_mico_wifi(mico_context, "MSFTLAB", "");

  mico_system_init(mico_context);

/* Comment out web-based config for WIFI due to resource limitation
  // start http server thread
  app_httpd_start();
  mico_rtos_delete_thread( NULL );
*/

  /* Output on debug serial port */
  os_helloworld_log( "Hello world! XXXX" );

  system_log("A -- Free memory %d bytes", MicoGetMemoryInfo()->free_memory); 
  duk_context *ctx = duk_create_heap_default();
  system_log("B -- Free memory %d bytes", MicoGetMemoryInfo()->free_memory); 
  os_helloworld_log( "DUK CONTEXT CREATED!" );

  duk_eval_string(ctx, "var a = 10 + 20; if (123) { if (123) {if (123) {if (123) {if (123) {if (123) {}}}}}}");
  os_helloworld_log( "EVAL STRING CALLED" );
  duk_destroy_heap(ctx);
  os_helloworld_log( "HEAP DESTROYED");
  system_log("C -- Free memory %d bytes", MicoGetMemoryInfo()->free_memory);

  /* Trigger MiCO system led available on most MiCOKit */
  while(1)
  {
      MicoGpioOutputTrigger( MICO_SYS_LED );
      mico_thread_sleep(1);
  }
}
