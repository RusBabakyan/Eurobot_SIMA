
from P2P_SIMA import P2P_RTU
import time
import struct
import math
from typing import Tuple
import numpy as np

# -----COMMANDS-----

CMD = { 'SET_SPEED':    0X00,
        'PING':         0X01,
        'SET_GRIPPER':      0X02,
        'GET_POS':     0X03,
        'SET_POS': 0X04,
        'RESTART':  0X05,
        'GET_SPEED':  0X06,
        'GET_POS_SPEED':  0X07,
        'GET_POS_DISTANCE':  0X08}

CMD_length = { 'SET_SPEED': 5,
               'PING':      0,
               'SET_GRIPPER':   1,
               'GET_POS':  0,
               'SET_POS': 12,
               'RESTART':  0,
               'GET_SPEED': 0,
               'GET_POS_SPEED': 0,
               'GET_POS_DISTANCE':  0}

RSP_length = { 'SET_SPEED': 0,
               'PING':      2+2+1,
               'SET_GRIPPER':   0,
               'GET_POS':  12,
               'SET_POS': 0,
               'RESTART':  0,
               'GET_SPEED': 8,
               'GET_POS_SPEED': 20,
               'GET_POS_DISTANCE':  15}

# -----COMMANDS-----

def clamp(n, minn, maxn):
    return max(min(maxn, n), minn)

class SIMA():
    def __init__(self, port_name: str, baud_rate: int):
        self.p2p_rtu = P2P_RTU(port_name, baud_rate)
        time.sleep(1)
        self.p2p_rtu.open_com_port()
        print('Ready')

        self.sensor_ready = False

    def set_speed(self, param: Tuple[int, int, bool]):
        # print(param)
        spd_l, spd_r, antistp = param
        pulse = 16383
        spd_l = clamp(spd_l, -pulse, pulse)
        spd_r = clamp(spd_r, -pulse, pulse)
        data = bytearray(struct.pack("h", spd_l))
        data += bytearray(struct.pack("h", spd_r))
        data += bytearray(struct.pack("?", antistp))
        self.p2p_rtu.send_request(CMD['SET_SPEED'], data)
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['SET_SPEED'])
        return cmd_error

    def set_gripper(self, state: int):
        data = bytearray(struct.pack("b", state))
        self.p2p_rtu.send_request(CMD['SET_GRIPPER'], data)
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['SET_GRIPPER'])
        return cmd_error

    def get_pos(self):
        self.p2p_rtu.send_request(CMD['GET_POS'], bytearray())
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['GET_POS'])
        if not cmd_error:
            X = struct.unpack('f', data[0:4])
            Y = struct.unpack('f', data[4:8])
            Angle = struct.unpack('f', data[8:12])
            print('\r' + f"x: {X[0]: .1f}, y: {Y[0]: .1f}, angle: {math.degrees(Angle[0]): .1f}, error: {cmd_error}",
                  end='', flush=True)
            return cmd_error, X[0], Y[0], math.degrees(Angle[0])
        else:
            print("lost message get pos", data)
            return 1, 0, 0, 0


    def set_pos(self, param: Tuple[float, float, float]):
        x, y, angle = param
        data = bytearray(struct.pack("f", x))
        data += bytearray(struct.pack("f", y))
        data += bytearray(struct.pack("f", angle))
        self.p2p_rtu.send_request(CMD['SET_POS'], data)
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['SET_POS'])
        return cmd_error

    def restart(self):
        self.p2p_rtu.send_request(CMD['RESTART'], bytearray())
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['RESTART'])
        self.sensor_ready = True
        return cmd_error

    def get_speed(self):
        self.p2p_rtu.send_request(CMD['GET_SPEED'], bytearray())
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['GET_SPEED'])
        if not cmd_error:
            speed_l = struct.unpack('f', data[0:4])[0]
            speed_r = struct.unpack('f', data[4:8])[0]
            return cmd_error, speed_l, speed_r
        else:
            return cmd_error, 0, 0

    def get_pos_speed(self):
        self.p2p_rtu.send_request(CMD['GET_POS_SPEED'], bytearray())
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['GET_POS_SPEED'])
        if not cmd_error:
            x = struct.unpack('f', data[0:4])[0]
            y = struct.unpack('f', data[4:8])[0]
            angle = struct.unpack('f', data[8:12])[0]
            angle = math.degrees(angle)
            speed_l = struct.unpack('f', data[12:16])[0]
            speed_r = struct.unpack('f', data[16:20])[0]
            return cmd_error, x, y, angle, speed_l, speed_r
        else:
            return cmd_error, 0, 0, 0, 0, 0

    def set_speed_f(self, param: Tuple[float, float, bool]):
        spd_l, spd_r, antistp = param
        spd_l = int(spd_l * self.coeff)
        spd_r = int(spd_l * self.coeff)
        return self.set_speed((spd_l, spd_r, antistp))

    def get_pos_distance(self):
        self.p2p_rtu.send_request(CMD['GET_POS_DISTANCE'], bytearray())
        cmd_error, data = self.p2p_rtu.receive_response(RSP_length['GET_POS_DISTANCE'])
        if not cmd_error:
            x = struct.unpack('f', data[0:4])[0]
            y = struct.unpack('f', data[4:8])[0]
            angle = struct.unpack('f', data[8:12])[0]
            angle = math.degrees(angle)
            stopflag = struct.unpack('?', data[12:13])[0]
            distance = struct.unpack('H', data[13:15])[0]
            return cmd_error, x, y, angle, distance, stopflag
        else:
            return cmd_error, 0, 0, 0, 0, 0

    # def turn(self, angle_target):
    #     print("Start turning")
    #     er, x, y, angle = self.get_pos()
    #     gap = 1
    #     k = 35
    #     speed = 2000
    #     while not (-gap < angle_target - angle < gap):
    #         er, x, y, angle = self.get_pos()
    #         angle_vel = int(k * (angle_target - angle))
    #         print('\r' + f"angle: {angle: .1f}, target: {angle_target}, Vel: {angle_vel}",
    #               end='', flush=True)
    #
    #         # self.set_speed((angle_vel, -angle_vel, True))
    #         self.set_speed((speed * np.sign(angle_vel), -speed * np.sign(angle_vel), True))
    #
    #     self.set_speed((0, 0, True))
    #     print("End turning")





