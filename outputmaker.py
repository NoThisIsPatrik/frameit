#!/usr/bin/python3
import os
import glob 

def main():
    dns = [ "./tmp__c_me_potassium.MOV" ]
    
    for dn in dns:
        fns = glob.glob(dn + "/*.csv")
        ofn = (dn.split('tmp_',1)[1]+".csv")
        ou = open(ofn,'w')
        for fn in fns: 
            ou.write(open(fn).read())
            # os.remove(f"{fn}")
        
if __name__=="__main__":
    main()
