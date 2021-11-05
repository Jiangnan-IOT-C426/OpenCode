from TCPClient import Select_Parameter
from TCPClient import Control_Operator
import time


interval = 100
for i in range(9):
	Control_Operator("Dio_0001");
    time.sleep(interval)


