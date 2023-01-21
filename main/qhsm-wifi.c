/*$file${../main::qhsm-wifi.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: wifi.qm
* File:  ${../main::qhsm-wifi.c}
*
* This code has been generated by QM 5.2.4 <www.state-machine.com/qm>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* SPDX-License-Identifier: GPL-3.0-or-later
*
* This generated code is open source software: you can redistribute it under
* the terms of the GNU General Public License as published by the Free
* Software Foundation.
*
* This code is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* NOTE:
* Alternatively, this generated code may be distributed under the terms
* of Quantum Leaps commercial licenses, which expressly supersede the GNU
* General Public License and are specifically designed for licensees
* interested in retaining the proprietary status of their code.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*/
/*$endhead${../main::qhsm-wifi.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#include "qpc.h"
#include "qhsm-wifi.h"

/*$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/* Check for the minimum required QP version */
#if (QP_VERSION < 700U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.0.0 or higher required
#endif
/*$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*$define${AOs::WiFi} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

/*${AOs::WiFi} .............................................................*/
WiFi WiFi_obj;

/*${AOs::WiFi::SM} .........................................................*/
QState WiFi_initial(WiFi * const me, void const * const par) {
    /*${AOs::WiFi::SM::initial} */
    return Q_TRAN(&WiFi_disconnected);
}

/*${AOs::WiFi::SM::disconnected} ...........................................*/
QState WiFi_disconnected(WiFi * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*${AOs::WiFi::SM::disconnected::CONNECTING} */
        case CONNECTING_SIG: {
            status_ = Q_TRAN(&WiFi_connecting);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

/*${AOs::WiFi::SM::connecting} .............................................*/
QState WiFi_connecting(WiFi * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*${AOs::WiFi::SM::connecting::CONNECTED} */
        case CONNECTED_SIG: {
            status_ = Q_TRAN(&WiFi_connected);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

/*${AOs::WiFi::SM::sta_start} ..............................................*/
QState WiFi_sta_start(WiFi * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*${AOs::WiFi::SM::sta_start::DISCONNECTED} */
        case DISCONNECTED_SIG: {
            status_ = Q_TRAN(&WiFi_disconnected);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

/*${AOs::WiFi::SM::sta_start::connected} ...................................*/
QState WiFi_connected(WiFi * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*${AOs::WiFi::SM::sta_start::connected::GOT_IP} */
        case GOT_IP_SIG: {
            status_ = Q_TRAN(&WiFi_got_ip);
            break;
        }
        default: {
            status_ = Q_SUPER(&WiFi_sta_start);
            break;
        }
    }
    return status_;
}

/*${AOs::WiFi::SM::sta_start::got_ip} ......................................*/
QState WiFi_got_ip(WiFi * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*${AOs::WiFi::SM::sta_start::got_ip::LOST_IP} */
        case LOST_IP_SIG: {
            status_ = Q_TRAN(&WiFi_connected);
            break;
        }
        default: {
            status_ = Q_SUPER(&WiFi_sta_start);
            break;
        }
    }
    return status_;
}
/*$enddef${AOs::WiFi} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*$define${AOs::WiFi_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

/*${AOs::WiFi_ctor} ........................................................*/
void WiFi_ctor(void) {
    QActive_ctor(&WiFi_obj.super, Q_STATE_CAST(&WiFi_initial));
}
/*$enddef${AOs::WiFi_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*$define${AOs::AO_WiFi} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

/*${AOs::AO_WiFi} ..........................................................*/
QActive * const AO_WiFi  = &WiFi_obj.super;
/*$enddef${AOs::AO_WiFi} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

