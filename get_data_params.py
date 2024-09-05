import os 
import re
import json

def read_set_metadata(set_dir):

    SEARCH_CHANNELS = r'srate\s*=\s*(\d+(?:\.\d+)?)'
    SEARCH_RATE = r'srate\s*=\s*(\d+(?:\.\d+)?)'

    set_files = [i for i in os.listdir(set_dir) if i.endswith(".set")]

    if not set_files: 
        print("No set files out in given data directory")

        return None

    set_file = set_files[0]
    fdt_file = set_file[:-4] + ".fdt"
    if not os.path.exists(os.path.join(set_dir, fdt_file)):
        print("Corresponding fdt file for {set_file} meterdata not found")

        return None

    with open(os.path.join(set_dir, set_file), 'r') as f:
        contents = f.read()

        match_channels = re.search(SEARCH_CHANNELS, contents)

        # if not match_channels: 

        channels = match_channels.group(1) # 0 < int <= 64

        match_rate = re.search(SEARCH_RATE, contents)

        rate = float(match_rate.group(1))


    output_data = {
        'channels': channels,
        'rate': rate,
        'set_file': set_file,
        'fdt_file': fdt_file
    }

    return output_data


if __name__ == "__main__": 

    data_dir = "path/to/data_dir"

    metadata = read_set_metadata(data_dir)

    with open('metadata.json', 'w') as f:
        json.dump(metadata, f)
        is_written = True

    with open('metadata.json', 'r') as f: 
        try: 
            written_data = json.load(f)
        except json.JSONDecodeError:
            is_written = False

    if written_data: 
        is_written = is_written



with open('src/metadata.h', 'w') as f: 
        # Write metadata to a C header file
    with open('metadata.h', 'w') as f:
        f.write("#ifndef METADATA_H\n")
        f.write("#define METADATA_H\n\n")
        f.write(f"#define CHANNELS {metadata['channels']}\n")
        f.write(f"#define RATE {metadata['rate']}\n")
        f.write(f"#define SET_FILE \"{metadata['set_file']}\"\n")
        f.write(f"#define FDT_FILE \"{metadata['fdt_file']}\"\n\n")
        f.write("#endif // METADATA_H\n")


