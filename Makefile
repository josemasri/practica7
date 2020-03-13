MPICC=mpicc
MRUN=mpirun

#Aqui esta la lista de parametros
PAR_I= -Wall -lm
PAR_LIB= -c -Wall -shared

#Archivos de cabecera
INC=.

#Archivos
PROG_01=ejercicio1
PROG_02=ejercicio2
PROG_03=ejercicio3

#Parametros
NODES=10

#Procesos
all: uno dos tres

uno:
	@echo "**************** COMPILADO CODIGO: $(PROG_01) ****************"
	$(MPICC) $(PAR_I) $(PROG_01).c -o $(PROG_01) -I$(INC)
dos:
	@echo "**************** COMPILADO CODIGO: $(PROG_02) ****************"
	$(MPICC) $(PAR_I) $(PROG_02).c -o $(PROG_02) -I$(INC)

tres:
	@echo "**************** COMPILADO CODIGO: $(PROG_03) ****************"
	$(MPICC) $(PAR_I) $(PROG_03).c -o $(PROG_03) -I$(INC)

rununo:
	@echo "**************** COMPILADO CODIGO: $(PROG_01) ****************"
	$(MPICC) $(PAR_I) $(PROG_01).c -o $(PROG_01) -I$(INC)
	$(MRUN) -np $(NODES) -hostfile maq $(PROG_01)

rundos:
	@echo "**************** COMPILADO CODIGO: $(PROG_02) ****************"
	$(MPICC) $(PAR_I) $(PROG_02).c -o $(PROG_02) -I$(INC)
	$(MRUN) -np $(NODES) -hostfile maq $(PROG_02)

runtes:
	@echo "**************** COMPILADO CODIGO: $(PROG_03) ****************"
	$(MPICC) $(PAR_I) $(PROG_03).c -o $(PROG_03) -I$(INC)
	$(MRUN) -np $(NODES) -hostfile maq $(PROG_03)

clean:
	rm $(PROG_01) $(PROG_02) $(PROG_03)