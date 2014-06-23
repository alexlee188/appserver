create trigger customer_update_trg after update on CUSTOMER
begin
  update CUSTOMER set lastupdatedon = datetime('NOW') where rowid = new.rowid;
end;
