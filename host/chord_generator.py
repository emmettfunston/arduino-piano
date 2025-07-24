import serial
from openai import OpenAI
import time
from dotenv import load_dotenv
load_dotenv()  # reads .env into os.environ
import os

client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

# Initialize serial connection to Arduino
arduino = serial.Serial('COM_PORT', 9600, timeout=1)  
time.sleep(2)  # Wait for connection to establish

def send_to_arduino(command):
  try:
    arduino.write((command + '\n').encode())
  except Exception as e:
    print(f"Error sending to Arduino: {e}")

def format_chords(raw_chords):
    # Ensure the chord format is space-separated and uppercase, e.g., "C E G"
    formatted_chords = ' '.join(raw_chords.upper().split())
    return formatted_chords

def get_chords_from_openai():
    noteArray = [
        "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
        "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", "C6"
    ]
    noteArrayStr = ', '.join(noteArray)
    prompt = f"The response should only include the chord progression no extra words. Only using the notes [{noteArrayStr}], provide a chord progression for a simple melody. Format the response as space-separated notes for each chord, e.g., 'C4 E5 G6#'. "
    
    response = client.chat.completions.create(model="gpt-3.5-turbo",
        messages=[
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": prompt}
        ])
    
    raw_chords = response.choices[0].message.content.strip()
    formatted_chords = format_chords(raw_chords)
    return formatted_chords

# Example usage
try:
    chords = get_chords_from_openai()
    send_to_arduino(f"{chords}")
    print(f"{chords}")
except Exception as e:
    print(f"Error: {e}")