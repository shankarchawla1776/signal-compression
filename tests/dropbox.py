import dropbox
import os
import logging


logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

def get_from_dropbox(filepaths, writing_dir, access_token):

    dbx = dropbox.Dropbox(access_token)


    if not os.path.exists(writing_dir):
        os.makedirs(writing_dir)


    for i in filepaths:

        try:
            metadata, res = dbx.files_download(i)

            local_filepath = os.path.join(writing_dir, os.path.basename(i))

            with open(local_filepath, 'wb') as f:
                f.write(res.content)

            logging.info(f"File Downloaded: {i} to {local_filepath}")

        except dropbox.exceptions.ApiError as e:
            logging.error("Dropbox ApiError: {e}")

        except Exception as e:
            logging.error("Script Error: {e}")


if __name__ == "__main__":

    access_token = os.getenv("DROPBOX_API_TOKEN")

    # .set -> metadata .fdt -> float32 signal data
    # dropbox api requires directories to start with /
    # data taken from https://openneuro.org/datasets/ds005207/versions/1.0.0
    filepaths = [
        "/Signal-Data/sub-001_ses-001_task-sleep_acq-PSG_eeg.set"
        "/Signal-Data/sub-001_ses-001_task-sleep_acq-PSG_eeg.fdt"
    ]

    writing_dir = "../data"

    get_from_dropbox(filepaths, writing_dir, access_token)

    logging.info("Download Done")
