clear all
close all
[wave,fs]=wavread('noise.wav'); % read file into memory */
[data,fs]=wavread('data.wav'); % read file into memory */
%sound(wave,fs); % see what it sounds like */
t=0:1/fs:(length(wave)-1)/fs; % and get sampling frequency */
td=0:1/fs:(length(data)-1)/fs; % and get sampling frequency */

%plot(t,wave);
 figure(1);
          plot(t,wave);
          title('Wave File');
          ylabel('Amplitude');
          xlabel('Length (in seconds)');
 figure(2);
          plot(td,data);
          title('Data File');
          ylabel('Amplitude');
          xlabel('Length (in seconds)');
 % graph it – try zooming while its up…not much visible until you do*/
n=length(wave)-1;
f=0:fs/n:fs;
nd=length(data)-1;
fd=0:fs/nd:fs;

wavefft=abs(fft(wave)); % perform Fourier Transform *

figure(3);
          plot(f,wavefft); % plot Fourier Transform */
          xlabel('Frequency in Hz');
          ylabel('Magnitude');
          title('The Wave FFT');
          
figure(4);
datafft=abs(fft(data)); % perform Fourier Transform *
          plot(fd,datafft); % plot Fourier Transform */
          xlabel('Frequency in Hz');
          ylabel('Magnitude');
          title('The Data FFT');