function [result]=Select_Parameter(parameter)
    out = parameter;
    tcpclient = tcpip('127.0.0.1', 8086);  %连接Socket服务器
    fopen(tcpclient);
    fwrite(tcpclient,"Select_"+out);       %发送查询指令到Socket服务器
    while(1)                               %轮询，直到有数据了再fread
        nBytes = get(tcpclient,'BytesAvailable');
        if nBytes>0
            break;
        end
    end
    receive = fread(tcpclient,nBytes);     %读取tcp服务器传来的数据
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
    
    fclose(tcpclient);    %关闭连接
    delete(tcpclient);    %删除连接
end
