function [result]=Select_Parameter(parameter)
    out = parameter;
    tcpclient = tcpip('127.0.0.1', 8086);  %Connect to Socket server
    fopen(tcpclient);
    fwrite(tcpclient,"Select_"+out);       %Send query command to Socket server
    while(1)                               % Polling, and then fread until there is data
        nBytes = get(tcpclient,'BytesAvailable');
        if nBytes>0
            break;
        end
    end
    receive = fread(tcpclient,nBytes);     %Read the data from the server
    receive = dec2hex(receive);
    
    if (parameter=="Tem")
        tem_16 = [receive(4,1),receive(4,2),receive(5,1),receive(5,2)];
        tem_10 = hex2dec(tem_16);
        result = tem_10/10;
    end
    
    if(parameter=="Hug")
        hum_16 = [receive(6,1),receive(6,2),receive(7,1),receive(7,2)];
        hum_10 = hex2dec(hum_16);
        result = hum_10/10;
    end

    if(parameter=="Lig")
        lig_16 = [receive(8,1),receive(8,2),receive(9,1),receive(9,2)];
        result = hex2dec(lig_16);
    end
    
    if(parameter=="Dio")
        dio_16 = [receive(10,1),receive(10,2),receive(11,1),receive(11,2)];
        result = hex2dec(dio_16);
    end

    if(parameter=="Flour")
        dio_16 = [receive(12,1),receive(12,2),receive(13,1),receive(13,2)];
        result = hex2dec(dio_16);
    end
    
    fclose(tcpclient);   
    delete(tcpclient);    
end
