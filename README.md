# JointQuest
![JointQuest Lobby](https://github.com/user-attachments/assets/a2235ced-c187-4e92-8c30-c8e11ae638e9)

JointQuest is a rehabilitation-focused fitness game developed by the Software Engineering department at Chonnam National University. It primarily targets assisting patients and elderly individuals, aiming to make sports rehabilitation enjoyable for users while helping them perform movements with correct posture.

[Demonstration Video](https://drive.google.com/file/d/1aLByF-InLGH0ghyFAvarlWySVCE7vd7z/view?usp=drive_link)

## Project Overview

This application uses pose estimation AI to track joint movements, enabling users to participate in rehabilitation exercises through gamified experiences. The system provides real-time feedback on posture and movement accuracy, ensuring optimal exercise performance.

## Architecture
![Pasted image 20240811211741](https://github.com/user-attachments/assets/373fbb0b-765d-4cea-a226-e14cfa422ae8)

The system uses a Python process for joint tracking via Mediapipe, and Unreal Engine receives this data to run the game.

## Features

1. **Single Camera Setup**: Requires only one webcam for full functionality
2. **Real-time Joint Tracking and Visual Feedback**: Incorporates webcam overlay on the gameplay interface and alerts users to incorrect postures
3. **Adaptive Gameplay Mechanics**: Adjusts game progression based on joint tracking data and posture accuracy
4. **Video Capture and Playback**: Delivers post-session statistics and video playback for user assessment

### Collaborators

- **박대형** - [GitHub Profile](https://github.com/ACUVEE)
- **신찬슬** - [GitHub Profile](https://github.com/ClockworkOwlKR)
- **박도영** - [GitHub Profile](https://github.com/doyoungp00)
- **정지웅** - [GitHub Profile](https://github.com/saintoong)


