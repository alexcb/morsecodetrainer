# morse code trainer

dependencies:
 pulse audio

converting wav:
 ls *.wav | grep -v _ | python-pipe 'x=line[:1]; print(f"sox {line} -r 16000 -c 1 -b 16 {x}_.wav tempo 1.5; mv {x}_.wav {x}.wav")' | bash
 
