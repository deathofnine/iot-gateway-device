#ifndef DHT11_H
#define DHT11_H
void mydht11_init(void);
int mydht11_read(char *humi,char *temp);
void mydht11_close(int i);
#endif // DHT11_H
