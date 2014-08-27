#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <wand/magick_wand.h>
int
sort_hist (PixelWand ** p, PixelWand ** q)
{
  size_t ip, iq;
  ip = PixelGetColorCount (*p);
  iq = PixelGetColorCount (*q);
  if (ip > iq)
    return (1);
  if (ip < iq)
    return (-1);
  return (0);

}

void
values_from_image (char * image_file)
{
  MagickWand *mw = NULL;
  // Comment out this define to use the region iterator instead of the Draw
  //#define USE_DRAW
  PixelWand **pixels = NULL;
  size_t i, num_colours;
  char *pc;
  FILE *fo = NULL;

  MagickWandGenesis ();

  /* Create a wand */
  mw = NewMagickWand ();

  /* Read the input image */
  MagickReadImage (mw, image_file);
  fo = fopen ("logo.txt", "w");
  pixels = MagickGetImageHistogram (mw, &num_colours);
  qsort(pixels,num_colours,sizeof(PixelWand *),sort_hist);
  int q = num_colours - 4;
  float red, green, blue;
  int red256, green256, blue256;
  for (i = q; i < num_colours; i++)
    {
      red = (float)(PixelGetRedQuantum(pixels[i])) * 256 / 65535 ;
      red256 = (int)red;
      green = (float)(PixelGetGreenQuantum(pixels[i])) * 256 / 65535 ;
      green256 = (int)green;
      blue = (float)(PixelGetBlueQuantum(pixels[i])) * 256 / 65535 ;
      blue256 = (int)blue;
      //printf("red %3d, green %3d, blue %3d\n", red256, green256, blue256);
      printf("%d,%d,%d\n", red256, green256, blue256);
/*
      fprintf (fo, "%6d: (", PixelGetColorCount (pixels[i]));
      fprintf (fo, "%d, ", PixelGetRedQuantum (pixels[i])/65535*256  );
      fprintf (fo, "%d, ", PixelGetGreenQuantum (pixels[i]));
      fprintf (fo, "%d, ", PixelGetBlueQuantum (pixels[i]));
      fprintf (fo, "%d)", PixelGetAlphaQuantum (pixels[i]));

      fprintf (fo, "\n");
*/
    }

  fclose (fo);
  // Free up the PixelWand array
  RelinquishMagickMemory (pixels);
  if (mw)
    mw = DestroyMagickWand (mw);

  MagickWandTerminus ();
}

int
main (int argc, char **argv)
{
  struct stat sb;
  
  if (argc != 2)
  {
    printf("You need to give me an image to work with.\n");
    exit(1);
  }
  
  if (stat(argv[1], &sb) == -1)
  {
    perror("stat");
    exit(1);
  }
  //values_from_image ( "/home/mark/Artwork/Gravitas.png");
  values_from_image ( argv[1] );
  return 0;
}
