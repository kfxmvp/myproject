import xml.dom.minidom
from ftplib import FTP

#读取XML初始化FTP参数
def ConfigInit():
    #源FTP路径
    global srcaddrval
    global srcportval
    global srcuserval
    global srcpasswdval
    global btssrcpathval
    global smcsrcpathval
    global sxcsrcpathval

    #目的FTP路径
    global dstaddrval
    global dstportval
    global dstuserval
    global dstpasswdval
    global btsdstpathval
    global smcdstpathval
    global sxcdstpathval

    global ftpsrc
    global ftpdst

    #从XML中读取FTP相关配置参数
    dom = xml.dom.minidom.parse('./Ftp.xml')
    root = dom.documentElement
    srcaddrlist = root.getElementsByTagName('SrcAddr')
    srcaddrval = srcaddrlist[0].firstChild.data
    srcportlist = root.getElementsByTagName('SrcPort')
    srcportval = srcportlist[0].firstChild.data
    srcuserlist = root.getElementsByTagName('SrcUser')
    srcuserval = srcuserlist[0].firstChild.data
    srcpasswdlist = root.getElementsByTagName('SrcPsw')
    srcpasswdval = srcpasswdlist[0].firstChild.data
    btssrcpathlist = root.getElementsByTagName('BtsSrcCdrPath')
    btssrcpathval = btssrcpathlist[0].firstChild.data
    smcsrcpathlist = root.getElementsByTagName('SmcSrcCdrPath')
    smcsrcpathval = smcsrcpathlist[0].firstChild.data
    sxcsrcpathlist = root.getElementsByTagName('SxcSrcCdrPath')
    sxcsrcpathval = sxcsrcpathlist[0].firstChild.data

    dstaddrlist = root.getElementsByTagName('DstAddr')
    dstaddrval = dstaddrlist[0].firstChild.data
    dstportlist = root.getElementsByTagName('DstPort')
    dstportval = dstportlist[0].firstChild.data
    dstuserlist = root.getElementsByTagName('DstUser')
    dstuserval = dstuserlist[0].firstChild.data
    dstpasswdlist = root.getElementsByTagName('SrcPsw')
    dstpasswdval = dstpasswdlist[0].firstChild.data
    sxcdstpathlist = root.getElementsByTagName('SxcDstCdrPath')
    sxcdstpathval = sxcdstpathlist[0].firstChild.data
    smcdstpathlist = root.getElementsByTagName('SmcDstCdrPath')
    smcdstpathval = smcdstpathlist[0].firstChild.data
    btsdstpathlist = root.getElementsByTagName('BtsDstCdrPath')
    btsdstpathval = btsdstpathlist[0].firstChild.data

    #初始化FTP
    ftpsrc=FTP()
    ftpsrc.set_debuglevel(0)
    ftpsrc.connect(srcaddrval,int(srcportval))
    ftpsrc.login(srcuserval,srcpasswdval)

    ftpdst=FTP()
    ftpdst.set_debuglevel(0)
    ftpdst.connect(dstaddrval,int(dstportval))
    ftpdst.login(dstuserval,dstpasswdval)

#移动srcpath的文件到dstpath对应目录下 ftp操作
def moveFile(srcpath,dstpath):
    global ftpsrc
    global ftpdst

    dirpath = ftpsrc.pwd() + srcpath
    srcdirlist = []
    dirlist = ftpsrc.nlst(dirpath)
    for tmpdir in dirlist:
        filelist = ftpsrc.nlst(tmpdir)
        for tmpfile in filelist:
            index = tmpdir.index('00')
            tmpdstdir = tmpdir[index:]
            sock1 = ftpsrc.transfercmd('RETR {}'.format(tmpfile))
            sock2 = ftpdst.transfercmd('STOR {}'.format(dstpath+tmpdstdir))
            flen = 0
            while 1:
                block = sock1.recv(1024)
                if len(block) == 0:
                    break
            flen += len(block)
            while len(block) > 0:
                sentlen = sock2.send(block)
                block = block[sentlen:]

            print("Transferred", flen, "bytes")

ConfigInit()
moveFile(sxcsrcpathval,sxcdstpathval)
moveFile(smcsrcpathval,smcdstpathval)
moveFile(btssrcpathval,btsdstpathval)
