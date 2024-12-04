#pragma once


typedef unsigned char dht22_sda_pin_size_t;
typedef unsigned char dht22_err_code;
typedef struct _dht22_object dht22_object;


struct _dht22_object * dht22_create_object(void);
void dht22_free_object(dht22_object *ob);
void dht22_set_sda_pin(dht22_object *ob, dht22_sda_pin_size_t pin_num);
void dht22_test(dht22_object *ob);


/*
 * File: DHT22_DRIVER_H
 */
