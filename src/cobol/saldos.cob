       IDENTIFICATION DIVISION.
       PROGRAM-ID. SALDOS.
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT TRANSAC ASSIGN TO "data/transacciones.dat"
               ORGANIZATION IS LINE SEQUENTIAL
               FILE STATUS IS WS-FS-IN.
           SELECT SALDOS ASSIGN TO "data/saldos.dat"
               ORGANIZATION IS LINE SEQUENTIAL.

       DATA DIVISION.
       FILE SECTION.
       FD  TRANSAC.
       01  REG-TRX.
           05 T-CUENTA        PIC 9(6).
           05 T-TIPO          PIC X.
           05 T-MONTO         PIC 9(9).
           05 T-FECHA         PIC 9(8).
       FD  SALDOS.
       01  REG-SAL.
           05 S-CUENTA        PIC 9(6).
           05 S-SALDO         PIC S9(11) SIGN LEADING SEPARATE.
           05 S-NTRX          PIC 9(5).

       WORKING-STORAGE SECTION.
       01  WS-FS-IN           PIC XX.
       01  WS-EOF             PIC X     VALUE "N".
       01  WS-N               PIC 9(3)  VALUE 0.
       01  WS-I               PIC 9(3).
       01  WS-IDX             PIC 9(3).
       01  WS-LEIDAS          PIC 9(7)  VALUE 0.
       01  TABLA.
           05 ENT OCCURS 200 TIMES.
              10 E-CUENTA     PIC 9(6).
              10 E-SALDO      PIC S9(11).
              10 E-N          PIC 9(5).

       PROCEDURE DIVISION.
       MAIN.
           OPEN INPUT TRANSAC
           IF WS-FS-IN NOT = "00"
              DISPLAY "[COBOL] No se pudo abrir transacciones.dat ("
                      WS-FS-IN ")"
              MOVE 1 TO RETURN-CODE
              STOP RUN
           END-IF
           PERFORM UNTIL WS-EOF = "S"
              READ TRANSAC
                 AT END MOVE "S" TO WS-EOF
                 NOT AT END
                    ADD 1 TO WS-LEIDAS
                    PERFORM ACUMULAR
              END-READ
           END-PERFORM
           CLOSE TRANSAC

           OPEN OUTPUT SALDOS
           PERFORM VARYING WS-I FROM 1 BY 1 UNTIL WS-I > WS-N
              MOVE E-CUENTA(WS-I) TO S-CUENTA
              MOVE E-SALDO(WS-I)  TO S-SALDO
              MOVE E-N(WS-I)      TO S-NTRX
              WRITE REG-SAL
           END-PERFORM
           CLOSE SALDOS

           DISPLAY "[COBOL] " WS-LEIDAS " transacciones -> "
                   WS-N " cuentas"
           STOP RUN.

       ACUMULAR.
           MOVE 0 TO WS-IDX
           PERFORM VARYING WS-I FROM 1 BY 1
                   UNTIL WS-I > WS-N OR WS-IDX > 0
              IF E-CUENTA(WS-I) = T-CUENTA
                 MOVE WS-I TO WS-IDX
              END-IF
           END-PERFORM
           IF WS-IDX = 0
              ADD 1 TO WS-N
              MOVE WS-N TO WS-IDX
              MOVE T-CUENTA TO E-CUENTA(WS-IDX)
              MOVE 0 TO E-SALDO(WS-IDX)
              MOVE 0 TO E-N(WS-IDX)
           END-IF
           ADD 1 TO E-N(WS-IDX)
           IF T-TIPO = "C"
              ADD T-MONTO TO E-SALDO(WS-IDX)
           ELSE
              SUBTRACT T-MONTO FROM E-SALDO(WS-IDX)
           END-IF.
