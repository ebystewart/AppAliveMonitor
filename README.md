# AppAliveMonitor
Application to monitor the state of apps managed by AppManager

                                                                               __________________
                                                                _______________|                 |
                                                               /               | Application 1   |
                                                              /                |                 |
                                                             /  message queue  -------------------
                                                            /                          :
_________________                      ___________________ /                   ________;__________
|               |     named pipe       |                 |/__message queue_____|                 |
|  APP Manager  |--------------------->|  Alive Monitor  |\                    | Application 2   |
|               |                      |                 | \                   |                 |
-----------------                      -------------------  \                  -------------------
                                                             \                         :
                                                              \                        :
                                                               \                       :
                                                                \  message queue       :
                                                                 \                     :
                                                                  \                    :
                                                                   \                   :
                                                                    \          ________'__________
                                                                     \         |                 |
                                                                      \________|  Application n  |
                                                                               |                 |
                                                                               -------------------