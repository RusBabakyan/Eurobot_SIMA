
from joystick import MyController
from SIMA_CLASS import SIMA

import pgzrun
WIDTH = 1500
HEIGHT = 1000

joy = MyController()
robot = SIMA('COM6', 57600) # Выбрать нужный COM порт
error = 0

background = Actor('table_small2', (WIDTH/2, HEIGHT/2))
actor = Actor('sima', (WIDTH/2, HEIGHT/2))

def draw():
    screen.fill((255, 255, 255))
    background.draw()
    actor.draw()

def update():
    global error
    error += robot.set_speed(joy.set_speed())
    if joy.set_gripper() <= 0:
        robot.set_gripper(joy.set_gripper())

    er, x, y, angle = robot.get_pos()
    actor.x = x/2
    actor.y = y/2
    actor.angle = -angle - 90
    error += er

    # print(robot.get_speed())

    if joy.B:
        robot.set_pos((WIDTH, HEIGHT, 0))

    if joy.Y:
        robot.sensor_init()
    return


pgzrun.go()


from joystick import MyController
from SIMA_CLASS import SIMA

joy = MyController()
robot = SIMA('COM6', 57600) # Выбрать нужный COM порт
while True:
    robot.set_speed(joy.set_speed())
    if joy.set_gripper() <= 0:
        robot.set_gripper(joy.set_gripper())

    if joy.Y:
        robot.sensor_init()

