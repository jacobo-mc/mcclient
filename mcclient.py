import pygame
import OpenGL
OpenGL.FORWARD_COMPATIBLE_ONLY = True
from OpenGL.GL import *
from OpenGL.GLU import *
from ctypes import c_float
from struct import pack


from time import time
import network.messages as messages
from network.connection import Connection
from world import World




def loadImage(image):
    textureSurface = pygame.image.load(image)

    textureData = pygame.image.tostring(textureSurface, 'RGBA', 1)

    width = textureSurface.get_width()
    height = textureSurface.get_height()

    texture = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, textureData)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

    return texture, width, height


if __name__ == '__main__':
    pygame.init()
    pygame.display.set_mode((800, 600), pygame.OPENGL | pygame.DOUBLEBUF)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
    glEnable(GL_DEPTH_TEST)
#    glEnable(GL_ALPHA_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glEnable(GL_TEXTURE_2D)
    glDisable(GL_LIGHTING)
    glEnableClientState(GL_VERTEX_ARRAY)
    glEnableClientState(GL_TEXTURE_COORD_ARRAY)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60, 800./600., 0.1, 128)


    truc = loadImage('mcdata/terrain.png')


    world = World()
    con  = Connection('Bot')
    con.connect('localhost')
    messages.KeepAlive().send(con.socket)

    message_pos = None

    last_ping = 0

    vertex = []
    texcoords = []
    x = None

    clock = pygame.time.Clock()

    while con.socket:
        for message in con.get_messages():
            if type(message) == messages.ChatMessage:
                print('[MSG] %s' % message.message)

            if type(message) == messages.BlockChange:
                pass#world.modify_block(message.x, message.y, message.z, message.block_type, message.block_metadata)

            if type(message) == messages.MultiBlockChange:
                for (ox, oy, oz), block_type, block_metadata in zip(message.coordinates, message.types, message.metadatas):
                    ox = ox + message.chunk_x * 16
                    oz = oz + message.chunk_z * 16
                    #world.modify_block(ox, oy, oz, block_type, block_metadata)

            if type(message) == messages.PreChunk:
                if message.mode == 1:
                    world.allocate_sector(message.x, message.z)
                else:
                    world.delete_sector(message.x, message.z)

            if type(message) == messages.MapChunk:
                world.map_chunk(message.x, message.y, message.z, message.size_x, message.size_y, message.size_z, message.compressed_data)


            if type(message) == messages.PlayerPosLook:
                print('[PlayerPosLook] %d, %d (%d), %d ; %f, %f'
                        % (message.x, message.y, message.stance, message.z,
                           message.yaw, message.pitch))
                x, y, z = message.x, message.stance, message.z
                message_pos = message
                message.send(con.socket)

            if type(message) == messages.UpdateHealth:
                print('[Vie] %d / 20' % message.health)

        if message_pos:
            message_pos.yaw = (10. * time()) % 360
            message_pos.pitch = 2

            message_pos.send(con.socket)
        else:
            messages.KeepAlive().send(con.socket)

        if x and world.needs_updating(x, y, z):
            vertex, texcoords = world.get_gl_faces(x, y, z)
            # Hack to speed up things, PyOpenGL sux
            oldtime = time()
            floats = pack('f' * (3 * len(vertex)), *(coord for point in vertex for coord in point))
            floats2 = pack('f' * (2 * len(vertex)), *(coord for point in texcoords for coord in point))
            glVertexPointer(3, GL_FLOAT, 0, floats)
            glTexCoordPointer(2, GL_FLOAT, 0, floats2)
            print('Time ellapsed: %f' % (time() - oldtime))
            print('Nb vertex drawn: %d' % (len(vertex)))

        if x:
            glClearColor(0.0, 0.0, 1.0, 0)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

            glMatrixMode(GL_MODELVIEW)
            glLoadIdentity()
            glRotated(message_pos.pitch, 1, 0, 0)
            glRotated(180 + message_pos.yaw, 0, 1, 0)
            glTranslated(-x, -y, -z)

            glDrawArrays(GL_QUADS, 0, len(vertex))

        pygame.display.flip()
        clock.tick(50)
