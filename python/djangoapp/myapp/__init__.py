from myapp.models import Person, Address

def init():
    doni = Person.objects.create(name='Doni', age=27, sex='m')
    ruby = Person.objects.create(name='Ruby', age=18, sex='f')
    Person.objects.create(name='Candy', age=20, sex='m')
    Person.objects.create(name='Bobby', age=20, sex='m')
    Person.objects.create(name='Monky', age=50, sex='f')
    Person.objects.create(name='Cici', age=2, sex='f')

    address = Address.objects.create(code="020")

