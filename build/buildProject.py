import os

fileTypes = {'cpp':8,'asm':2,'c':1}

location = '../../'
dirL = './examples/'

def addIncPath(f, n: str):
    if os.path.isdir(n):
        f.write(location + n + ';')
        print(location + n)
        for d in os.listdir(n):
            addIncPath(f,n+'/'+d)


def addGroup(f,g_name,f_type,dir_name):
    f.write(' '*8+'<Group>\n')
    f.write(' '*10+'<GroupName>' + g_name + '</GroupName>\n')

    f.write(' '*8+'</Group>\n')

def addFileContent(f_name, f):
    with open(f_name) as file:
        data = file.read()
        f.write(data)


def create_project(name):
    if not os.path.exists(dirL + name):
        os.mkdir(dirL + name)
        os.mkdir(dirL + name+'/src')

    with open(dirL + name+'/src/main.cpp', 'w') as f:
        f.write('#include "common.h"\n\n')
        f.write('void main(void)\n{\n\twhile(1);\n}\n')
    with open(dirL + name+'/'+name+'.uvprojx', 'w') as f:
        addFileContent('./build/start2name.txt', f)
        f.write(name)

        addFileContent('./build/name2inc.txt', f)
        print('include path:')
        addIncPath(f,'CMSIS')
        addIncPath(f,'MM32SPIN2xx_p_LIB')
        addIncPath(f,'WyCppHal')
        addIncPath(f,'WyLib')
        f.write('./;')
        f.write('./src')

        addFileContent('./build/inc2group.txt', f)
        addGroup(f)

        addFileContent('./build/group2end.txt', f)
        f.write(name)

        addFileContent('./build/end.txt', f)


if __name__ == '__main__':
    n = input("请输入项目名称: ")
    create_project(n)
