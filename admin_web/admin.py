import cherrypy
import os
import json
from jinja2 import Environment, FileSystemLoader
from aux import debug
#env = Environment(loader=FileSystemLoader('templates'))
dir = os.listdir(os.getcwd() + '/static')

class adminResource(object):
  exposed = True
  def __init__(self,env):
    self.env=env
    pass
  
    
#sketch/ abre sketch
  def index(self):
    tmpl = self.env.get_template("admin.html")
    return tmpl.render()         
  index.exposed = True

  
      
def convertBool(stringBool):
  return stringBool in ['true', '1', 't', 'y', 'yes', 'True']
      