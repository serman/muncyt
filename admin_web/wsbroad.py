import cherrypy
import os
import json
from jinja2 import Environment, FileSystemLoader
from aux import debug
from ws4py.websocket import WebSocket

#env = Environment(loader=FileSystemLoader('templates'))
dir = os.listdir(os.getcwd() + '/static')

class wsbroad(WebSocket):
  def __init__(self,model,env):
    self.myModel=model
    self.env=env
    pass
  #https://github.com/Lawouach/WebSocket-for-Python/blob/master/example/echo_cherrypy_server.py
  def received_message(self, m):
      cherrypy.engine.publish('websocket-broadcast', m)

  def closed(self, code, reason="A client left the room without a proper explanation."):
      cherrypy.engine.publish('websocket-broadcast', TextMessage(reason))  
    
#sketch/ abre sketch
  def index(self,error=""):
    m_error=error;
    m_code=""
    #dir = os.listdir(os.getcwd() + '/static')
    #remote = cherrypy.request.headerMap.get("Remote-Addr", "") 
    tmpl = self.env.get_template("editor.html") 
    return tmpl.render(error=m_error,code=m_code,dbdata={},environment=cherrypy.config.get('server.environment'))          
  index.exposed = True 
  

      
def convertBool(stringBool):
  return stringBool in ['true', '1', 't', 'y', 'yes', 'True']
      