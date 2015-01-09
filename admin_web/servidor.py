"""

"""

import cherrypy
import simplejson
import string
import os
import json
import base64
#import OSC
from json import dumps, loads, JSONEncoder, JSONDecoder
from jinja2 import Environment, FileSystemLoader
env = Environment(loader=FileSystemLoader('templates'))
import logging
import time
logger = logging.getLogger('mopidy.frontends.http')
#import model
#import threading
from admin import adminResource
import sys
from cherrypy.process import plugins
from passwd import userpassdict
#from ws4py.server.cherrypyserver import WebSocketPlugin, WebSocketTool
#from ws4py.websocket import WebSocket
#from ws4py.messaging import TextMessage
#from ws4py.websocket import EchoWebSocket
#from threading import Timer
import datetime
#from wsbroad import wsbroad;
import geoip
import subprocess
import sys
s=None    
class Server(object):        
  def __init__(self):    
    #self.myModel=model.videoManager();    
    cherrypy.config["tools.encode.on"] = True
    cherrypy.config["tools.encode.encoding"] = "utf-8"
    #self.client = OSC.OSCClient()
    #self.client.connect( ('127.0.0.1', 12000) ) # note that the argument is a tupple and not two arguments
    self.hosts={"espejo1":"192.168.98.21","espejo2":"192.168.98.22","espejo3":"192.168.98.23 ","mesa":"127.0.0.1","techo":"192.168.98.25"}
    #elf.rcvFromP5()
    #self.sendToP5()
    #self.speedHsk=0;

    self.isConnected=False;
    #self.lock = threading.RLock()    
    self.lastAccess=datetime.datetime.now();
    #self.accessCounter=0;
    #self.numberOfUsers=0;
    self.listofID=[]
    #self.t = Timer(10.0, self.checkConnections)
    #self.t = cherrypy.process.plugins.BackgroundTask(15,self.checkConnections)   
    #self.t2 = cherrypy.process.plugins.BackgroundTask(5,self.toProcessing)     
    #self.motor=0; self.arduino=0; self.speed=0;
    #self.t.start()
    #self.t2.start()

    #self.t.start()

  @cherrypy.expose 
  def index(self):
    #if(cherrypy.request.headers['Host']=="tes.uncoded.es"  or cherrypy.request.headers['Host']=="www.tes.uncoded.es"   ):
    #        raise cherrypy.InternalRedirect("/indexmobile")
    m_error=""
    m_code=""
    dir = os.listdir(os.getcwd() + '/static')
    #remote = cherrypy.request.headerMap.get("Remote-Addr", "") 
    tmpl = env.get_template("index.html")
    return tmpl.render(directories=dir,error=m_error,code=m_code,environment=cherrypy.config.get('server.environment'))  
  


  @cherrypy.expose 
  def indexmobile(self):
    m_error=""
    m_code=""
    dir = os.listdir(os.getcwd() + '/static')
    #remote = cherrypy.request.headerMap.get("Remote-Addr", "") 
    tmpl = env.get_template("indexmobile.html")
    return tmpl.render(directories=dir,error=m_error,code=m_code,environment=cherrypy.config.get('server.environment'))  
  
  @cherrypy.expose 
  @cherrypy.tools.json_out()
  def reset(self,computer_id):
    if (computer_id=="mesa"):
        print "reset mesa"
        subprocess.check_call(['/Users/escaner/quitrestart.sh'])
    else:
        subprocess.check_call(['ssh', "instalaciones@"+self.hosts[computer_id], '/quitrestart.sh'])
    print computer_id
    return {'ok':True } 

  @cherrypy.expose 
  @cherrypy.tools.json_out()
  def restartMesa(self):
    subprocess.check_call(['/Users/escaner/restartmesa.sh'])
    return {'ok':True }     

  @cherrypy.expose
  @cherrypy.tools.json_out()
  def resetEspejoApp(self,copmuter_id):
    subprocess.check_call(['ssh', "instalaciones@"+self.hosts[computer_id], 'killall -9 kinectTouchSurface_2Debug'])
    return {'ok':True }
#------------------- main -- 

import os.path
class Root(object):
    pass
s


    

class threadCloser(plugins.SimplePlugin):

    def start(self):
        pass

    def stop(self):
        pass
        #t.cancel()

if __name__ == '__main__':
    tutconf = os.path.join(os.path.dirname(__file__), 'fiveminutesoffame.conf')
    current_dir = os.path.dirname(os.path.abspath(__file__))
    #cherrypy.config.update(conf)
    server=Server()
    cherrypy.engine.threadcloser = threadCloser(cherrypy.engine)  
    cherrypy.engine.threadcloser.subscribe()
    server.admin  = adminResource( env)       
#    app = cherrypy.tree.mount(server, config=tutconf)
    
    #cherrypy.quickstart(server,config=tutconf) 
   # WebSocketPlugin(cherrypy.engine).subscribe()
    #cherrypy.tools.websocket = WebSocketTool()

    checkpassword = cherrypy.lib.auth_basic.checkpassword_dict(userpassdict)
    basic_auth = {'tools.auth_basic.on': True,
                  'tools.auth_basic.realm': 'earth',
                  'tools.auth_basic.checkpassword': checkpassword,
    }
    confdict = { '/admin' : basic_auth,
                
                    
                
             
            }


#########
    cherrypy.config.update(tutconf)
    cherrypy.config.update(confdict)
    #https://ws4py.readthedocs.org/en/latest/sources/servertutorial/#cherrypy

 
    app = cherrypy.tree.mount(server, '/', tutconf)
    app.merge(confdict)
    
    if hasattr(cherrypy.engine, "signal_handler"):
        cherrypy.engine.signal_handler.subscribe()
    if hasattr(cherrypy.engine, "console_control_handler"):
        cherrypy.engine.console_control_handler.subscribe()
       
    cherrypy.engine.start()
    cherrypy.engine.block()    

    
    

   
else:
    # This branch is for the test suite; you can ignore it.
    cherrypy.tree.mount(Server(), config=tutconf)
    


