cmd_/home/kerem/Odev4/Module.symvers := sed 's/\.ko$$/\.o/' /home/kerem/Odev4/modules.order | scripts/mod/modpost -m -a  -o /home/kerem/Odev4/Module.symvers -e -i Module.symvers   -T -
