#ifndef WEBHOOK_API_H
#define WEBHOOK_API_H

#include <Arduino.h>

bool sendWebhookData(float h, float ph, bool n, bool p, bool k, float precipitation, bool bombaLigada);

#endif // WEBHOOK_API_H
