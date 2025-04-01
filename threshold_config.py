import os

vcasn_val = 45
ithr_val = 60

for vcasn in range(45, 56, 1):
	for ithr in range(50, 65, 5):
		output_file = open('config/Config_vcasn' + str(vcasn) + '_ithr' + str(ithr) + '.cfg', 'w')
		output_file.write('DEVICE CHIPMOSAIC\nCHIPID 0\nNMASKSTAGES 512\nPIXPERREGION 32\nBOARDVERSION 1\nADDRESS 192.168.168.5\nCONTROLLATENCYMODE 0\nPOLLINGDATATIMEOUT 500\nDATALINKPOLARITY 0\nLINKSPEED 400\nPLLSTAGES 0\nNTRIG 240000\nIBIAS 64\nIDB 64\nVCASP 86\nVCLIP 0\nVPULSEH 170\nVRESETP 117\nVRESETD 147\n')
		output_file.write('ITHR ' + str(ithr) + '\n')
		output_file.write('VCASN ' + str(vcasn) + '\n')
		output_file.write('VCASN2 ' + str(vcasn+12) + '\n')
		output_file.write('PULSEDELAY 10000\nPULSEDURATION 500\nSTROBEDELAYCHIP 20\nSTROBEDURATION 10000')

		output_file.close()