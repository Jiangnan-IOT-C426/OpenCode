function [result] = Control_Operator(parameter)
    if(strlength(parameter)==5)
        out = [parameter(1:4),'000',parameter(5)];
    elseif(strlength(parameter)==6)
        out = [parameter(1:4),'00',parameter(5:6)];
    else
        out = parameter;
    end
   
    tcpclient = tcpip('127.0.0.1', 8086);
    fopen(tcpclient);
    fwrite(tcpclient, "Control_"+out);
 
    while(1)                            
        nBytes = get(tcpclient,'BytesAvailable');
        if nBytes>0
            break;
        end
    end
    in = fread(tcpclient,nBytes);     %读取tcp服务器传来的数据
    if(in(4)==4 && in(6)==1)
        result='Lig_1';
    else
        result='Lig_0';
    end
   
    %关闭连接
    fclose(tcpclient);
    delete(tcpclient);
    
end