import pygame
#import os
import OSC
#http://www.pygame.org/docs/ref/joystick.html
#os.environ['SDL_VIDEODRIVER'] = 'dummy'
# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)

CONNECTED = 0;
# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputing the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def print1(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height
        
    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15
        
    def indent(self):
        self.x += 10
        
    def unindent(self):
        self.x -= 10
    

client = OSC.OSCClient()
client.connect( ('127.0.0.1', 12000) ) # note that the argument is a tupple and not two arguments

pygame.init()
msg = OSC.OSCMessage()
msg.setAddress("/connect") 
client.send(msg)


# Set the width and height of the screen [width,height]
size = [500, 700]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("miniPongRemoteControll")

#Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()
      


# Initialize the joysticks
pygame.joystick.init()
    
# Get ready to print
textPrint = TextPrint()

# -------- Main Program Loop -----------
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
        
        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
        if event.type == pygame.JOYBUTTONDOWN:
            try:         
               # print        str(event)          
                msg = OSC.OSCMessage()
                msg.setAddress("/joystick/"+str(event.joy)+"/JOYBUTTONDOWN" )
                msg.append(event.button) 
                client.send(msg)
            except OSC.OSCClientError:
                pass
            except :
                print "another exception"

        if event.type == pygame.JOYBUTTONUP:
            try:
               # print         str(event)   
                msg = OSC.OSCMessage()
                msg.setAddress("/joystick/"+str(event.joy)+"/JOYBUTTONUP" )
                msg.append(event.button) 
                client.send(msg)
            except OSC.OSCClientError:
                pass
            except :
                print "another exception"
        if event.type == pygame.JOYAXISMOTION:                      
            try:
                #print str(event)              
                msg = OSC.OSCMessage()
                msg.setAddress("/joystick/"+str(event.joy)+"/axis/"+str(event.axis) )
                msg.append(event.value) 
                #client.send(msg)
            except OSC.OSCClientError:
                pass
            except :
                print "another exception"
     # Limit to 40 frames per second
    clock.tick(20) 
    #continue
    # DRAWING STEP
    # First, clear the screen to white. Don't put other drawing commands
    # above this, or they will be erased with this command.
    screen.fill(WHITE)
    textPrint.reset()

    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()

    textPrint.print1(screen, "Number of joysticks: {}".format(joystick_count) )
    textPrint.indent()
    
    # For each joystick:
    for j in range(joystick_count):
        joystick = pygame.joystick.Joystick(j)
        joystick.init()
    
        textPrint.print1(screen, "Joystick {}".format(j) )
        textPrint.indent()
    
        # Get the name from the OS for the controller/joystick
        name = joystick.get_name()
        textPrint.print1(screen, "Joystick name: {}".format(name) )
        
        # Usually axis run in pairs, up/down for one, and left/right for
        # the other.
        axes = joystick.get_numaxes()
        textPrint.print1(screen, "Number of axes: {}".format(axes) )
        textPrint.indent()
        
        for i in range( axes ):
            axis = round(joystick.get_axis( i ))
            textPrint.print1(screen, "Axis {} value: {:>6.3f}".format(i, axis) )
            try:
                #print str(event)              
                msg = OSC.OSCMessage()
                msg.setAddress("/joystick/"+str(j)+"/axis/"+str(i) )
                msg.append(axis)
                client.send(msg)
            except OSC.OSCClientError:
                pass
            except :
                print "another exception"
            pygame.time.wait(25)
        continue        
        textPrint.unindent()
            
        buttons = joystick.get_numbuttons()
        textPrint.print1(screen, "Number of buttons: {}".format(buttons) )
        textPrint.indent()

        for i in range( buttons ):
            if(i>3):
                break
            button = joystick.get_button( i )
            textPrint.print1(screen, "Button {:>2} value: {}".format(i,button) )
            # try:
            #     if(button>0):
            #         msg = OSC.OSCMessage()
            #         msg.setAddress("/joystick/"+str(j)+"/button/"+str(i) )
            #         msg.append(button) 
            #         client.send(msg)
            # except OSC.OSCClientError:
            #     pass
            # except :
            #     print( "another exception")

        textPrint.unindent()
            
        # Hat switch. All or nothing for direction, not like joysticks.
        # Value comes back in an array.
        hats = joystick.get_numhats()
        textPrint.print1(screen, "Number of hats: {}".format(hats) )
        textPrint.indent()

        for i in range( hats ):
            hat = joystick.get_hat( i )
            textPrint.print1(screen, "Hat {} value: {}".format(i, str(hat)) )
        textPrint.unindent()
        
        textPrint.unindent()

    
    # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
    
    # Go ahead and update the screen with what we've drawn.
    #pygame.display.flip()

    
    
# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit ()

