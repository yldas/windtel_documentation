import { TestBed } from '@angular/core/testing';

import { Esp32Service } from './esp32.service';

describe('Esp32Service', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: Esp32Service = TestBed.get(Esp32Service);
    expect(service).toBeTruthy();
  });
});
