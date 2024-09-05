import re

def dts_to_header(dts_file, header_file):
    with open(dts_file, 'r') as dts, open(header_file, 'w') as header:
        header.write('#ifndef BCM2711_DTS_H_\n')
        header.write('#define BCM2711_DTS_H_\n\n')

        for line in dts:
            match = re.match(r'^\s*(\w+)\s*=\s*<\s*(0x[0-9a-fA-F]+)\s*>;', line)
            if match:
                name, value = match.groups()
                header.write(f'#define {name.upper()} {value}\n')

        header.write('\n#endif // BCM2711_DTS_H_\n')

dts_to_header('bcm2711-rpi-4-b.dts', 'bcm2711_dts.h')
