import sys
from astropy.io import fits
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt

def view_fits(filename):
    with fits.open(filename) as hdul:
        print(hdul.info())

        image_data = hdul[0].data

        if image_data is None:
            print("no image data honey")
            return
        
        if image_data.ndim == 2:
            plt.imshow(image_data, cmap='gray', origin='lower')
            plt.colorbar()
            plt.title(filename)
            output_file = filename.replace(".fits", '.png')
            plt.savefig(output_file, dpi=300, bbox_inches='tight')
        else:
            print(f"unsupported image dimensions: {image_data.shape}")

def main():
    view_fits("detection_images/detection_chi2pos_SWLW_A1.fits")

if __name__ == "__main__":
    main()
