import sys

'''
########################################
FIRST VRAIBLE IS THE NAMe .c is added automaticly
SECOND IW WTHERE OR NOT WE WONT MATH 1 IS WE WANT IT
thid is wthere we want it to be compact(1) or not 0
then the argumantens 
const {type} {var=val}
-c {var} a..b
every thing after -r is a resulut 
#######################################
'''

#name const type var=val float int re 
from functools import total_ordering

#file_name=sys.argv[1]

identefiers={'int':'d','float':'f','char':'c'}
var_types={'char':0,'int':1,'float':2}
variables=[]

@total_ordering
class variable:
    def __init__(self,name,type_,is_const,is_reult,range_='a..b'):
        self.type_=type_
        self.is_const=is_const
        self.name=name
        self.is_result=is_reult
        self.range_=range_
    def __eq__(self, other):
        return self.is_const and other.is_const and self.type_==other.type
    def __lt__(self, other):
        return self.is_const and not(other.is_const) or var_types[self.type_]<var_types[other.type_]
            


arg=4
is_const=False
is_result=False
while arg != len(sys.argv):
    if sys.argv[arg]=='-r':
        is_result=True
    elif sys.argv[arg]=='const':
        arg+=1
        current_type=sys.argv[arg]
        is_const=True
    elif sys.argv[arg] in var_types:
        current_type=sys.argv[arg]
        is_const=False
    elif sys.argv[arg]=='-c':
        range_=sys.argv[arg+2]
        variables.append(variable(sys.argv[arg+1],current_type,is_const,is_result,range_))
        arg+=2
    else:
        variables.append(variable(sys.argv[arg],current_type,is_const,is_result))
    arg+=1


Base='#include <stdio.h>\n'+'#include <math.h>\n'*int(sys.argv[2])+'int main()\n{'
c_vars_lines=['\n\tconst '+c_type_+' '+','.join([c_var.name for c_var in variables if (c_var.is_const and c_var.type_==c_type_)])+';' for c_type_ in var_types]
vars_lines=['\n\t'+c_type_+' '+','.join([var.name for var in variables if (not(var.is_const) and var.type_==c_type_)])+';' for c_type_ in var_types]
c_vars_lines=[i for i in c_vars_lines if i[-2]!=' ']#clean variabless items
vars_lines=[i for i in vars_lines if i[-2]!=' ']
definitions=''.join(c_vars_lines)+''.join(vars_lines)
logique=''
results=['\n\tprintf("les valures:','"','']
var_list=['\n\tprintf("donner les valures des','scanf("','']
variables.sort()
for var in variables:
    if not(var.is_const):
        if var.is_result:
	    logique+='\n\t{}=;'.format(var.name)
       	    if int(sys.argv[3]):#compact mode
       	        results[0]+=' {0}=%{1}'.format(var.name,identefiers[var.type_])
       	        results[1]+=',{0}'.format(var.name)
       	    else:
	 	
            	logique+='\n\tprintf("valure de {0} est %{1}\\n",{0});'.format(var.name,identefiers[var.type_])
        else:
            cond=int(var.range_!='a..b')
            ranges=var.range_.split('..')
            if cond or not(int(sys.argv[3])):
            	logique+='\n\tdo { '*cond
            	logique+='\n\tprintf("donner la valure de {}: ");'.format(var.name)
            	logique+='\n\tscanf("%{}",&{});'.format(identefiers[var.type_],var.name)
            	logique+='\n\t{3}\n while({0}>={1} && {0}<={2});'.format(var.name,ranges[0],ranges[-1],'}')*cond
            else:
            	var_list[0]+=' '+var.name
            	var_list[1]+='%'+identefiers[var.type_]
            	var_list[2]+=',&'+var.name
if int(sys.argv[3]):logique=(var_list[0]+'");\n\t'+var_list[1]+'"'+var_list[2]+');\n\t')*(len(var_list[2])!=0)+logique+(''.join(results)+');\n\t')*(len(results[1])!=0)
           
file_ = open(sys.argv[1]+'.c', "w")
file_.write(Base+definitions+logique+'\n}')
file_.close()

