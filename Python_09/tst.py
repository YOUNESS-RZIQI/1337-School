from pydantic import Field, BaseModel

class A(BaseModel):
    name: str = Field()



a = A()
