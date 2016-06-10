# Source code of the library to keep alive a battery power bank.

A project in collaboration with makerspace - Faculty of Computer Science at
the Free University of Bolzano. See also
[makerspace | learning by doing](http://makerspace.inf.unibz.it/).

```
 m  a  k  e  r  s  p  a  c  e  .  i  n  f  .  u  n  i  b  z  .  i  t  
                                                                      
8888888888888888888888888888888888888888888888888888888888888888888888
                                                                      
               8                                                      
               8                                                      
YoYoYo. .oPYo. 8  .o  .oPYo. YoYo. .oPYo. 8oPYo. .oPYo. .oPYo. .oPYo. 
8' 8' 8 .oooo8 8oP'   8oooo8 8  `  Yb..`  8    8 .oooo8 8   `  8oooo8 
8  8  8 8    8 8 `b.  8.  .  8      .'Yb. 8    8 8    8 8   .  8.  .  
8  8  8 `YooP8 8  `o. `Yooo' 8     `YooP' 8YooP' `YooP8 `YooP' `Yooo' 
                                          8                           
                                          8                           
                                                                      
8888888888888888888888888888888888888888888888888888888888888888888888
                                                                      
 c  o  m  p  u  t  e  r    s  c  i  e  n  c  e    f  a  c  u  l  t  y 
```

### Usage Instructions

Use the following circuit on your Arduino board:
```
Battery Power Bank Load Circuit:

    5V o-----------.
                   |
                  .-.
                  | | 100R
                  | | 1/4W
                  '-'
                   |
           1k      |
           ___   |/  BC547
batPin o--|___|--|
                 |>  NPN 100mA
          1/4W     |
                   |
                  ===
                  GND

(created by AACircuit v1.28.6 beta 04/19/05 www.tech-chat.de)
```
and setup in your code the library with `PowerBankControl::begin(batPin);`,
where *batPin* is the digital pin number where you have attached the cricuit.

In the loop function call `PowerBankControl::keepBatteryAlive();` continuously.
