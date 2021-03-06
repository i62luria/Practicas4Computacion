function [frecuencia, freordenada] = cripto_ana_orden(v)
    format shortg;
    frecuencia = zeros(27,2);
    freordenada = zeros(27,2);
    numeros = letranumero(v);
    for i=1:27
        X = (numeros==i-1);
        frec = sum(X);
        frecuencia(i,1) = frec/length(v);
        frecuencia(i,2) = floor(i-1);
    end
    
    freordenada = sortrows(frecuencia,1,'descend');