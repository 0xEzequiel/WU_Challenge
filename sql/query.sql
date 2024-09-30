declare @fechaLimite date = dateadd(month, -12, getdate())

select      c.ID, c.Nombre, c.Apellido,
            sum(v.Importe) as totalGastado
from        Clientes c
inner join  Ventas v 
on          c.ID = v.Id_cliente
where       v.Fecha >= @fechaLimite
group by    c.ID, c.Nombre, c.Apellido
having      sum(v.Importe) > 100000;