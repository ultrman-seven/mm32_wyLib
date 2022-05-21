import os

def addIncPath(f):
  pass

def addGroup(f):
  pass

def addFileContent(f_name,f):
  with open(f_name) as file:
    data = file.read()
    f.write(data)

def create_project(name):
  if not os.path.exists(name):    
    os.mkdir(name)

  with open(name+'/'+name+'.uvprojx','w') as f:
    addFileContent('./build/start2name.txt',f)
    f.write(name)

    addFileContent('./build/name2inc.txt',f)
    addIncPath(f)

    addFileContent('./build/inc2group.txt',f)
    addGroup(f)

    addFileContent('./build/group2end.txt',f)
    f.write(name)

    addFileContent('./build/end.txt',f)
    
if __name__ == '__main__':
    n = input("请输入项目名称: ")
    create_project(n)
    