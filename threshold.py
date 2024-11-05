import os

output_file = open('build/Data/threshold.csv', 'w')

file_list = os.listdir('/mnt/raw/cluster_size/threshold/')
number = 0

output_file.write('Number, RawPath, ConfigPath, DataPath, OutputPath, GraphPath\n')

raw_path = '/mnt/raw/cluster_size/threshold/'
config_path = '/mnt/raw/cluster_size/threshold/'
data_path = '/mnt/homes/ychoi/PIDproject/entry_test/preprocessed/threshold/'
proprecessed_path = '/mnt/homes/ychoi/PIDproject/entry_test/plots/threshold/'
graph_path = '/mnt/homes/ychoi/PIDproject/entry_test/graph/threshold/'


for file in file_list:
	if file.endswith('.dat'):
		date_str = file[14:27]
		number_str = f'{number:06}'
		raw_file = raw_path + 'ThresholdScan_' + date_str + '.dat'
		config_file = config_path + 'ScanConfig_' + date_str + '.cfg'
		data_file = data_path + 'Threshold_' + date_str + '.root'
		proprecessed_file = proprecessed_path + 'Threshold_' + date_str
		graph_file = graph_path + 'Threshold_' + date_str + '.root'
		line = number_str + ', ' + raw_file + ', ' + config_file + ', ' + data_file + ', ' + proprecessed_file + ', ' + graph_file + '\n'
		
		output_file.write(line)
		number = number + 1