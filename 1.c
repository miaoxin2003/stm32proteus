// DHT11数据读取函数
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;

    DHT11_Rst(); // 发送复位信号

    if(DHT11_Check() == 0) // 检查DHT11响应
    {
        for(i = 0; i < 5; i++) // 读取5字节数据
        {
            buf[i] = DHT11_Read_Byte();
        }

        // 校验数据完整性
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0]; // 湿度整数部分
            *temp = buf[2]; // 温度整数部分
            return 0;       // 读取成功
        }
    }
    return 1; // 读取失败
}
