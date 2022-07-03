#include<Windows.h>
#include"libcelt/celt.h"
#include<iostream>
#include<stdlib.h>
#include<string>

#define BUF_SIZE 1024*1024

int main(int argc,char * argv[]) {
	typedef CELTMode*(*pcelt_mode_create)(int sampling_rate, int channels, int* error);
	typedef CELTDecoder*(*pcelt_decoder_create_custom)(const CELTMode* mode, int channels, int* error);
	typedef int(*pcelt_decode)(CELTDecoder* st, const unsigned char* data, int len, celt_int16* pcm, int frame_size);

	HINSTANCE hDLL;
	hDLL = LoadLibrary("vaudio_celt.dll");
	std::cout<<GetLastError();

	pcelt_mode_create Celt_mode_create = (pcelt_mode_create)GetProcAddress(hDLL, "celt_mode_create");
	pcelt_decoder_create_custom Celt_decoder_create_custom = (pcelt_decoder_create_custom)GetProcAddress(hDLL, "celt_decoder_create_custom");
	pcelt_decode Celt_decode = (pcelt_decode)GetProcAddress(hDLL, "celt_decode");

	std::cout << "ok" << std::endl;

	unsigned char* buf = (unsigned char*)malloc(BUF_SIZE * sizeof(unsigned char));
	//unsigned char buf[BUF_SIZE];

	//22050, 512, 64
	const unsigned int SAMPLE_RATE = 22050;
	const unsigned int FRAME_SIZE = 512;
	const unsigned int Packet_Size = 64;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			FILE* f = fopen(argv[i], "rb");
			if (f == NULL) {
				return 1;
			}

			size_t read = fread(buf, 1, BUF_SIZE, f);
			fclose(f);

			CELTMode* dm = Celt_mode_create(SAMPLE_RATE, FRAME_SIZE, 0);
			CELTDecoder* d = Celt_decoder_create_custom(dm, 1, NULL);

			size_t outsize = (read / Packet_Size) * FRAME_SIZE * sizeof(celt_int16);
			celt_int16* pcmout = (celt_int16*)malloc(outsize);

			size_t done = 0;
			int frames = 0;

			for (; done < read; done += Packet_Size, frames++) {
				int ret = 0;
				if ((ret = Celt_decode(d, buf + done, Packet_Size, pcmout + frames * FRAME_SIZE, FRAME_SIZE)) < 0) {
					fprintf(stderr, "unable to decode... > %d (at %d/%d)\n", ret, done, read);
					//return 1;
				}
				printf("Done %d/%d (to %ld/%ld)\n", done, read, frames * FRAME_SIZE * 2, outsize);
			}

			std::string output_filename = argv[i];
			//replace extension name
			auto dot_pos = output_filename.find_last_of('.');
			output_filename = dot_pos >= 0 ? (output_filename.substr(0,dot_pos)+".raw") : output_filename.append(".raw");

			FILE* file_p = fopen(output_filename.c_str(), "wb");
			size_t written = fwrite(pcmout, outsize, 1, file_p);
			fclose(file_p);
			free(pcmout);
			std::cout << "ok" << std::endl;
		}
	}

	free(buf);
	FreeLibrary(hDLL);
	return 0;	
}