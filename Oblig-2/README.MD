Skriv en README-fil hvor du forklarer følgende punkter:
• Hvordan du leser superblokkfilen fra disk og laster inodene inn i minnet.
    Først er det lurt å vite hva denne "superblokkfilen" består av. Filen består av
    "inoder" hvor man har delt opp datatypene slik som vist i obligteksten gjennom "rot-directory". 
    Hensikten ved å lese denne filen, er at vi skal tilordne "struct inode" verdiene gjennom denne filen. 
    Dette blir vist gjennom en fremstilling av hvordan rot-directory blir lagd ved at størrelsen 
    er 20 byte ved å lage denne structen. Dermed så må man kunne lese inn selve "superblokkfilen" 
    hvor man tilordner de ulike feltene i "struct inode" som f.eks, 'name_len', '*name', 'file_size' osv. 
    Derfor så må vi benytte av fread-funksjonen til å lese inn "superblokkfilen" og lage de ulike inodene 
    basert på om vi har en fil eller directory å jobbe med. 
    
• Eventuelle implementasjonskrav som ikke er oppfylt
    Alt er blitt implementert

• Eventuelle deler av implementasjonen som avviker fra prekoden. Dersom du for ek- sempel oppretter egne filer, forklar hva hensikten er.
    Har ikke lagd noen egne filer

• Eventuelle tester som feiler og hva du tror årsaken kan være.
    Alt går som det skal