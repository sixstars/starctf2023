'''

${]}=0
${!;*}=1
${-``}=2
${=$``}=3
${ ]}=4
${!}=5
${#.}=6
${(}=7
${)``}=8
${``*%}=9
${$%}='[CHar]'


'''

with open(r'fake_code.txt','r') as rf:
    data=rf.read()
# data='chiper'
with open('output.txt','w') as wf:
    output_string=''
    for char in data:
        ascii_value=ord(char)
        output_string+=f'[CHar]{ascii_value}+'
    output_string=output_string[:-1]
    output_string=output_string.replace('0','${]}')    
    output_string=output_string.replace('1','${!;*}')    
    output_string=output_string.replace('2','${*@ }')    
    output_string=output_string.replace('3','${=$``}')    
    output_string=output_string.replace('4','${ ]}')    
    output_string=output_string.replace('5','${!}')    
    output_string=output_string.replace('6','${#.}')    
    output_string=output_string.replace('7','${(}')    
    output_string=output_string.replace('8','${)``}')    
    output_string=output_string.replace('9','${``*%}')    
    output_string=output_string.replace('[CHar]','${$%}')   
    output_string+='' 
    wf.write(output_string)
