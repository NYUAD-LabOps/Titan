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

/***********************************************************************************************************************
 * Copyright [2017] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Device Stack                                                        */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/ 
/*                                                                        */ 
/*  COMPONENT DEFINITION                                   RELEASE        */ 
/*                                                                        */ 
/*    ux_device_stack.h                                   PORTABLE C      */ 
/*                                                           5.9          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron, Express Logic Inc.                                   */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file defines the equivalences for the USBX Device Stack        */ 
/*    component.                                                          */ 
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
/*                                            resulting in version 5.9    */
/*                                                                        */ 
/**************************************************************************/ 

/***********************************************************************************
 * @brief  This file defines the equivalences for the USBX Device Stack componenet.
 ***********************************************************************************/

#ifndef UX_DEVICE_STACK_H
#define UX_DEVICE_STACK_H

/* Common macro for SSP header files. There is also a corresponding SSP_FOOTER macro at the end of this file. */
SSP_HEADER

/* Define USB Device Stack prototypes.  */

UINT    _ux_device_stack_alternate_setting_get(ULONG interface_value);
UINT    _ux_device_stack_alternate_setting_set(ULONG interface_value, ULONG alternate_setting_value);
UINT    _ux_device_stack_class_register(UCHAR *class_name,
                    UINT (*class_entry_function)(struct UX_SLAVE_CLASS_COMMAND_STRUCT *),
                    ULONG configuration_number,
                    ULONG interface_number,
                    VOID *parameter);
UINT    _ux_device_stack_clear_feature(ULONG request_type, ULONG request_value, ULONG request_index);
UINT    _ux_device_stack_configuration_get(VOID);
UINT    _ux_device_stack_configuration_set(ULONG configuration_value);
UINT    _ux_device_stack_control_request_process(UX_SLAVE_TRANSFER *transfer_request);
UINT    _ux_device_stack_descriptor_send(ULONG descriptor_type, ULONG request_index, ULONG host_length);
UINT    _ux_device_stack_disconnect(VOID);
UINT    _ux_device_stack_endpoint_stall(UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_device_stack_get_status(ULONG request_type, ULONG request_index, ULONG request_length);
UINT    _ux_device_stack_host_wakeup(VOID);
UINT    _ux_device_stack_initialize(UCHAR * device_framework_high_speed, ULONG device_framework_length_high_speed,
                    UCHAR * device_framework_full_speed, ULONG device_framework_length_full_speed,
                    UCHAR * string_framework, ULONG string_framework_length,
                    UCHAR * language_id_framework, ULONG language_id_framework_length,
                    UINT (*ux_system_slave_change_function)(ULONG));
UINT    _ux_device_stack_interface_delete(UX_SLAVE_INTERFACE *interface);
UINT    _ux_device_stack_interface_get(UINT interface_value);
UINT    _ux_device_stack_interface_set(UCHAR * device_framework, ULONG device_framework_length,
                    ULONG alternate_setting_value);
UINT    _ux_device_stack_interface_start(UX_SLAVE_INTERFACE *interface);
UINT    _ux_device_stack_set_feature(ULONG request_type, ULONG request_value, ULONG request_index);
UINT    _ux_device_stack_transfer_all_request_abort(UX_SLAVE_ENDPOINT *endpoint, ULONG completion_code);
UINT    _ux_device_stack_transfer_request(UX_SLAVE_TRANSFER *transfer_request, ULONG slave_length, ULONG host_length);
UINT    _ux_device_stack_transfer_abort(UX_SLAVE_TRANSFER *transfer_request, ULONG completion_code);
UINT    _ux_device_stack_class_unregister(UCHAR *class_name, UINT (*class_entry_function)(struct UX_SLAVE_CLASS_COMMAND_STRUCT *));
UINT    _ux_device_stack_microsoft_extension_register(ULONG vendor_request, UINT (*vendor_request_function)(ULONG, ULONG, ULONG, ULONG, UCHAR *, ULONG *));
UINT    _ux_device_stack_uninitialize(VOID);
/* Common macro for SSP header files. There is also a corresponding SSP_HEADER macro at the top of this file. */
SSP_FOOTER

#endif

