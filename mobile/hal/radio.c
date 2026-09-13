#include "radio.h"
#include "../../kernel/arch/x86_64/console.h"

static const struct jagx_radio_ops* radio_ops = 0;
static int data_enabled = 0;
static int airplane = 0;
static int call_active = 0;

void jagx_radio_register(const struct jagx_radio_ops* ops) {
    radio_ops = ops;
    console_write("[HAL] Radio/modem registered\n");
}

int jagx_mobile_data_set(int enabled) {
    data_enabled = enabled ? 1 : 0;
    if (!radio_ops || !radio_ops->set_data_enabled) {
        console_write(enabled ? "[HAL] Mobile data ON (stub)\n" : "[HAL] Mobile data OFF (stub)\n");
        return 0;
    }
    return radio_ops->set_data_enabled(enabled);
}

int jagx_airplane_set(int enabled) {
    airplane = enabled ? 1 : 0;
    if (!radio_ops || !radio_ops->set_power) {
        console_write(enabled ? "[HAL] Airplane ON (stub)\n" : "[HAL] Airplane OFF (stub)\n");
        return 0;
    }
    return radio_ops->set_power(enabled ? JAGX_RADIO_AIRPLANE : JAGX_RADIO_ON);
}

int jagx_sms_send(const char* number, const char* text) {
    if (!radio_ops || !radio_ops->sms_send) {
        console_write("[HAL] SMS send needs modem driver\n");
        return -1;
    }
    return radio_ops->sms_send(number, text);
}

int jagx_call_dial(const char* number) {
    if (airplane) {
        console_write("[HAL] Call blocked — airplane mode\n");
        return -1;
    }
    if (!radio_ops || !radio_ops->call_dial) {
        console_write("[HAL] Call dial (stub, no baseband)\n");
        call_active = 1;
        (void)number;
        return -1; /* stub: UI may still show a lab session */
    }
    return radio_ops->call_dial(number);
}

int jagx_call_answer(void) {
    if (!radio_ops || !radio_ops->call_answer) {
        call_active = 1;
        return 0;
    }
    return radio_ops->call_answer();
}

int jagx_call_hangup(void) {
    call_active = 0;
    if (!radio_ops || !radio_ops->call_hangup) return 0;
    return radio_ops->call_hangup();
}

int jagx_radio_signal(int* dbm) {
    if (dbm) *dbm = radio_ops && radio_ops->get_signal ? 0 : -1;
    if (!radio_ops || !radio_ops->get_signal) return -1;
    return radio_ops->get_signal(dbm);
}

int jagx_sim_ready(void) {
    if (!radio_ops || !radio_ops->sim_ready) return 0;
    return radio_ops->sim_ready();
}
