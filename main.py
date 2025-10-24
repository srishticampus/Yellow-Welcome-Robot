from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import openai

# Replace with your OpenAI key
openai.api_key = "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"  

app = FastAPI()

# Allow cross-origin requests (from browser)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"]
)

class Message(BaseModel):
    prompt: str

@app.post("/chat")
async def chat(message: Message):
    try:
        response = openai.ChatCompletion.create(
            model="gpt-4o-mini",
            messages=[
                {"role": "system", "content": "You are a helpful assistant that answers in 1-2 sentences."},
                {"role": "user", "content": message.prompt}
            ]
        )
        reply = response.choices[0].message["content"].strip()
        return {"reply": reply}
    except Exception as e:
        return {"reply": f"Error: {e}"}
