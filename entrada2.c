{ programa le uma sequencia de números inteiros e encontra o maior 
}
program exemplo;
    integer num, maior, cont, qtd;
begin
    read(qtd);
    cont := 0;
    maior := 0; #inicializa a variavel maior com 0
    while ( cont < qtd ) do
    begin
        read(num);
        if( num > maior ) then
            maior := num;
        cont := cont + 1
    end;
    write(maior)
end.