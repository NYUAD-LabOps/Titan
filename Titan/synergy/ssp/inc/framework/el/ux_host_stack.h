/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2019 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Host Stack                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  COMPONENT DEFINITION                                   RELEASE        */ 
/*                                                                        */ 
/*    ux_host_stack.h                                     PORTABLE C      */ 
/*                                                           5.9          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains all the header and extern functions used by the  */
/*    USBX Host Stack component.                                          */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  07-01-2007     TCRG                     Initial Version 5.0           */ 
/*  11-11-2008     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.2    */ 
/*  07-10-2009     TCRG                     Modified comment(s), and      */ 
/*                                            added trace logic,          */ 
/*                                            resulting in version 5.3    */ 
/*  06-13-2010     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.4    */ 
/*  09-01-2011     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.5    */ 
/*  10-10-2012     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.6    */ 
/*  06-01-2014     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.7    */ 
/*  06-01-2017     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.8    */ 
/*  08-15-2019     TCRG                     Modified comment(s),          */ 
/*                                            removed no use function,    */
/*                                            added function prototype,   */
/*                                            resulting in version 5.9    */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef UX_HOST_STACK_H
#define UX_HOST_STACK_H


/* Define Host Stack component function prototypes.  */

VOID    _ux_host_stack_bandwidth_release(UX_HCD *hcd, UX_ENDPOINT *endpoint);
VOID    _ux_host_stack_bandwidth_claim(UX_HCD *hcd, UX_ENDPOINT *endpoint);
UINT    _ux_host_stack_bandwidth_check(UX_HCD *hcd, UX_ENDPOINT *endpoint);
UX_HOST_CLASS * _ux_host_stack_class_call(UX_HOST_CLASS_COMMAND *class_command);
UINT    _ux_host_stack_class_device_scan(UX_DEVICE *device);
UINT    _ux_host_stack_class_get(UCHAR *class_name, UX_HOST_CLASS **class);
UINT    _ux_host_stack_class_instance_destroy(UX_HOST_CLASS *class, VOID *class_instance);
UINT    _ux_host_stack_class_instance_create(UX_HOST_CLASS *class, VOID *class_instance);
UINT    _ux_host_stack_class_instance_get(UX_HOST_CLASS *class, UINT class_index, VOID **class_instance);
UINT    _ux_host_stack_class_instance_verify(UCHAR *class_name, VOID *class_instance);
UINT    _ux_host_stack_class_interface_scan(UX_DEVICE *device);
UINT    _ux_host_stack_class_register(UCHAR *class_name,
                        UINT (*class_entry_function)(struct UX_HOST_CLASS_COMMAND_STRUCT *));
UINT    _ux_host_stack_configuration_descriptor_parse(UX_DEVICE *device, UX_CONFIGURATION *configuration, UINT configuration_index);
UINT    _ux_host_stack_configuration_enumerate(UX_DEVICE *device);
UINT    _ux_host_stack_configuration_instance_create(UX_CONFIGURATION *configuration);
VOID    _ux_host_stack_configuration_instance_delete(UX_CONFIGURATION *configuration);
UINT    _ux_host_stack_configuration_interface_get(UX_CONFIGURATION *configuration, 
                                                UINT interface_index, UINT alternate_setting_index,
                                                UX_INTERFACE **interface);
UINT    _ux_host_stack_configuration_set(UX_CONFIGURATION *configuration);
VOID    _ux_host_stack_delay_ms(ULONG time);
UINT    _ux_host_stack_device_address_set(UX_DEVICE *device);
UINT    _ux_host_stack_device_configuration_get(UX_DEVICE *device, UINT configuration_index,
                                                        UX_CONFIGURATION **configuration);
UINT    _ux_host_stack_device_configuration_select(UX_CONFIGURATION *configuration);
UINT    _ux_host_stack_device_configuration_reset(UX_DEVICE *device);
UINT    _ux_host_stack_device_descriptor_read(UX_DEVICE *device);
UINT    _ux_host_stack_device_get(ULONG device_index, UX_DEVICE **device);
UINT    _ux_host_stack_device_remove(UX_HCD *hcd, UX_DEVICE *parent, UINT port_index);
UINT    _ux_host_stack_device_resources_free(UX_DEVICE *device);
UINT    _ux_host_stack_endpoint_instance_create(UX_ENDPOINT *endpoint);
VOID    _ux_host_stack_endpoint_instance_delete(UX_ENDPOINT *endpoint);
UINT    _ux_host_stack_endpoint_reset(UX_ENDPOINT *endpoint);
UINT    _ux_host_stack_endpoint_transfer_abort(UX_ENDPOINT *endpoint);
VOID    _ux_host_stack_enum_thread_entry(ULONG input);
UINT    _ux_host_stack_hcd_register(UCHAR *hcd_name,
                                    UINT (*hcd_init_function)(struct UX_HCD_STRUCT *), ULONG hcd_param1, ULONG hcd_param2);
VOID    _ux_host_stack_hcd_thread_entry(ULONG input);
UINT    _ux_host_stack_hcd_transfer_request(UX_TRANSFER *transfer_request);
UINT    _ux_host_stack_initialize(UINT (*ux_system_host_change_function)(ULONG, UX_HOST_CLASS *, VOID *));
UINT    _ux_host_stack_interface_endpoint_get(UX_INTERFACE *interface, UINT endpoint_index, UX_ENDPOINT **endpoint);
UINT    _ux_host_stack_interface_instance_create(UX_INTERFACE *interface);
VOID    _ux_host_stack_interface_instance_delete(UX_INTERFACE *interface);
UINT    _ux_host_stack_interface_set(UX_INTERFACE *interface);
UINT    _ux_host_stack_interface_setting_select(UX_INTERFACE *interface);
UINT    _ux_host_stack_interfaces_scan(UX_CONFIGURATION *configuration, UCHAR * descriptor);
VOID    _ux_host_stack_new_configuration_create(UX_DEVICE *device, UX_CONFIGURATION *configuration);
UX_DEVICE  *_ux_host_stack_new_device_get(VOID);
UINT    _ux_host_stack_new_device_create(UX_HCD *hcd, UX_DEVICE *device_owner, 
                                UINT port_index, UINT device_speed,
                                UINT port_max_power);
UINT    _ux_host_stack_new_endpoint_create(UX_INTERFACE *interface, UCHAR * interface_endpoint);
UINT    _ux_host_stack_new_interface_create(UX_CONFIGURATION *configuration, UCHAR * descriptor, ULONG length);
VOID    _ux_host_stack_rh_change_process(VOID);
UINT    _ux_host_stack_rh_device_extraction(UX_HCD *hcd, UINT port_index);
UINT    _ux_host_stack_rh_device_insertion(UX_HCD *hcd, UINT port_index);
UINT    _ux_host_stack_transfer_request(UX_TRANSFER *transfer_request);
UINT    _ux_host_stack_transfer_request_abort(UX_TRANSFER *transfer_request);
VOID    _ux_host_stack_hnp_polling_thread_entry(ULONG id);
UINT    _ux_host_stack_role_swap(UX_DEVICE *device);

#endif

