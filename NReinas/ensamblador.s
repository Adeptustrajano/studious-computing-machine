    AREA codigo, CODE
    EXPORT resolverReinas

resolverReinas   
    PUSH {lr, r11}
    MOV r11, sp
    MOV r2, #0 ; fila
    PUSH {r0, r1, r2}
    BL colocarReina
    ADD sp, sp, #12
    POP {r11, pc}

;SUBRUTINA COLOCARREINAS
colocarReina
    PUSH {lr, r11}
    MOV r11, sp
    PUSH {r4, r5, r6, r8, r9}
    LDR r4, [r11, #8]   ; N
    LDR r5, [r11, #12]  ; solucion vector
    LDR r8, [r11, #16]  ; fila
    CMP r8, r4          ; Si hemos colocado N reinas
    BGE llamar_tablero  ; Mostrar y detener
    MOV r6, #0          ; columna = 0
bucle    
    CMP r6, r4          ; columna >= N?
    BGE fin_bucle
    STR r6, [r5, r8, LSL #2]  ; Colocar reina en la columna `r6` de la fila `r8`
    
    SUB sp, sp, #4      ; Reservar espacio para el resultado
    PUSH {r5, r8} 
    BL comprobar         ; Comprobar si la colocación es válida
    ADD sp, sp, #8      ; desapilar comporbar
    pop{r9}
    CMP r9, #0          ; Si la colocación no es válida, probar siguiente columna
    BEQ siguiente_columna
    
    ADD r9, r8, #1      ; Colocar reina en la siguiente fila, uso el r9 que ya no lo uso más luego
    PUSH {r4, r5, r9}
    BL colocarReina
    ADD sp, sp, #12

siguiente_columna
    ADD r6, r6, #1      ; Probar la siguiente columna
    B bucle

llamar_tablero
    PUSH {r4, r5} 
    BL mostrarTablero
    ADD sp, sp, #8
fin_bucle
    POP {r4, r5, r6, r8, r9}
    POP {r11, pc}
    
	
;SUBRUTINA COMPROBAR
comprobar
    PUSH {lr, r11}
    MOV r11, sp
    PUSH {r0, r1, r2, r3, r4, r5, r6, r7}  ; Guardamos los registros usados en comprobar

    MOV r0, #0                ; i = 0
    LDR r1, [r11, #12]          ; r1 = fila
    LDR r2, [r11, #8]           ; r2 = @v solucion

bucle_comprobar
    CMP r0, r1                 ; Compara i con fila
    BGE valido                 ; Si i >= fila, es válido
    LDR r3, [r2, r0, LSL #2]   ; r3 = reinas[i]
    LDR r4, [r2, r1, LSL #2]   ; r4 = reinas[fila]
    SUB r5, r1, r0             ; r5 = fila - i
    SUB r6, r3, r4             ; r6 = reinas[fila] - reinas[i]

    CMP r6, #0                 ; Compara si están en la misma columna
    BEQ invalido               ; Si están en la misma columna, es inválido

    ; Verificación de la diagonal
	cmp r6, #0
	rsblt r6, r6, #0
    CMP r5, r6                 ; Compara |fila - i| con |columna[fila] - columna[i]|
    BEQ invalido               ; Si son iguales, están en la misma diagonal (es inválido)

    ADD r0, r0, #1             ; i++ 
    B bucle_comprobar           ; Continúa con el siguiente valor de i

valido
    MOV r7, #1                 ; true (validación exitosa)
    B fin_comprobar

invalido
    MOV r7, #0                 ; false (colocación inválida)

fin_comprobar
    STR r7, [r11, #16]         ; Almacena en la memoria de comprobación (resultado)
    POP {r0, r1, r2, r3, r4, r5, r6, r7}  ; Restauramos los registros
    POP {r11, pc}              ; Regresamos de la subrutina

	
;SUBRUTINA DEL TABLERO
mostrarTablero 
    PUSH {lr, r11}
    MOV r11, sp
    PUSH {r4, r5, r6, r7, r8, r9, r10}
    LDR r4, [r11, #8]       ; N
    LDR r5, [r11, #12]      ; solucion vector
    LDR r9, =0x40007E00     ; Memoria donde se imprime
    MOV r6, #0              ; Fila inicial
bucle_filas
    CMP r6, r4
    BGE fin_tablero
    MOV r7, #0              ; Columna inicial
bucle_columnas
    CMP r7, r4
    BGE fin_fila
    LDR r8, [r5, r6, LSL #2] ; Obtener la columna de la reina en fila `r6`
    CMP r7, r8
    MOVEQ r10, #'#'         ; Reina 
    MOVNE r10, #'-'         ; Sin reina
    STRB r10, [r9], #1      ; Imprimir carácter en memoria
    ADD r7, r7, #1          ; Col++
    B bucle_columnas
fin_fila
    ADD r9, r9, #32         ; Mover a la siguiente línea en memoria
    SUB r9, r9, r4          ; -n
    ADD r6, r6, #1
    B bucle_filas
fin_tablero
    POP {r4, r5, r6, r7, r8, r9, r10}
    POP {r11, pc}
	END