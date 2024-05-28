import serial


class P2P_RTU():
    def __init__(self, port_name: str, baud_rate: int) -> None:
        self.port = serial.Serial()
        self.port_name = port_name
        self.baud_rate = baud_rate

    def open_com_port(self) -> None:
        if self.port.isOpen() == True:
            self.port.close()
        self.port = serial.Serial(self.port_name, self.baud_rate, timeout=0.1, xonxoff=False, rtscts=False,
                              dsrdtr=False)
        self.port.setRTS(False)
        self.port.setDTR(False)



    def close_com_port(self) -> None:
        if self.port.isOpen() == True:
            self.port.close()

    def __calc_crc(self, packet: bytearray) -> int:
        crc = 0
        # for ik in range(0,packet[2]+2):
        for ik in range(0,len(packet)):
            crc += packet[ik]
        # crc = ~crc
        crc &= 0xFF
        # print(f'crc: {crc}')
        return crc

    def __verify_response(self, response: bytearray, response_length: bytes) -> int:
        if len(response) < response_length:
            return 1
        crc = self.__calc_crc(response[:-1])
        if crc != response[-1]:
            return 1
        return 0

    def send_request(self, cmd: bytes, data: bytearray) -> None:
        start_byte = ord('!')
        request = bytearray([start_byte,start_byte, cmd]) + data
        request += bytearray([self.__calc_crc(request[2:])])
        # print(request)
        self.port.flushInput()
        self.port.write(request)

    def receive_response(self, data_length: bytes) -> list:
        response = self.port.read(data_length + 2)
        # print(response)
        # print(f'len of response: {len(response)}')
        error = self.__verify_response(response, data_length + 2)
        return error, response[1:-1]

